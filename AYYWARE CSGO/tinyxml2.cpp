/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "tinyxml2.h"

#include <new>		// yes, this one new style header, is in the Android SDK.
#if defined(ANDROID_NDK) || defined(__QNXNTO__)
#   include <stddef.h>
#   include <stdarg.h>
#else
#   include <cstddef>
#   include <cstdarg>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1400 ) && (!defined WINCE)
	// Microsoft Visual Studio, version 2005 and higher. Not WinCE.
	/*int _snprintf_s(
	   char *buffer,
	   size_t sizeOfBuffer,
	   size_t count,
	   const char *format [,
		  argument] ...
	);*/
	static inline int TIXML_SNPRINTF( char* buffer, size_t size, const char* format, ... )
	{
		va_list va;
		va_start( va, format );
		int result = vsnprintf_s( buffer, size, _TRUNCATE, format, va );
		va_end( va );
		return result;
	}

	static inline int TIXML_VSNPRINTF( char* buffer, size_t size, const char* format, va_list va )
	{
		int result = vsnprintf_s( buffer, size, _TRUNCATE, format, va );
		return result;
	}

	#define TIXML_VSCPRINTF	_vscprintf
	#define TIXML_SSCANF	sscanf_s
#elif defined _MSC_VER
	// Microsoft Visual Studio 2003 and earlier or WinCE
	#define TIXML_SNPRINTF	_snprintf
	#define TIXML_VSNPRINTF _vsnprintf
	#define TIXML_SSCANF	sscanf
	#if (_MSC_VER < 1400 ) && (!defined WINCE)
		// Microsoft Visual Studio 2003 and not WinCE.
		#define TIXML_VSCPRINTF   _vscprintf // VS2003's C runtime has this, but VC6 C runtime or WinCE SDK doesn't have.
	#else
		// Microsoft Visual Studio 2003 and earlier or WinCE.
		static inline int TIXML_VSCPRINTF( const char* format, va_list va )
		{
			int len = 512;
			for (;;) {
				len = len*2;
				char* str = new char[len]();
				const int required = _vsnprintf(str, len, format, va);
				delete[] str;
				if ( required != -1 ) {
					TIXMLASSERT( required >= 0 );
					len = required;
					break;
				}
			}
			TIXMLASSERT( len >= 0 );
			return len;
		}
	#endif
#else
	// GCC version 3 and higher
	//#warning( "Using sn* functions." )
	#define TIXML_SNPRINTF	snprintf
	#define TIXML_VSNPRINTF	vsnprintf
	static inline int TIXML_VSCPRINTF( const char* format, va_list va )
	{
		int len = vsnprintf( 0, 0, format, va );
		TIXMLASSERT( len >= 0 );
		return len;
	}
	#define TIXML_SSCANF   sscanf
#endif


static const char LINE_FEED				= (char)0x0a;			// all line endings are normalized to LF
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN		= (char)0x0d;			// CR gets filtered out
static const char CR = CARRIAGE_RETURN;
static const char SINGLE_QUOTE			= '\'';
static const char DOUBLE_QUOTE			= '\"';

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
//	ef bb bf (Microsoft "lead bytes") - designates UTF-8

static const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
static const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
static const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

namespace tinyxml2
{

struct Entity {
    const char* pattern;
    int length;
    char value;
};

static const int NUM_ENTITIES = 5;
static const Entity entities[NUM_ENTITIES] = {
    { "quot", 4,	DOUBLE_QUOTE },
    { "amp", 3,		'&'  },
    { "apos", 4,	SINGLE_QUOTE },
    { "lt",	2, 		'<'	 },
    { "gt",	2,		'>'	 }
};


StrPair::~StrPair()
{
    Reset();
}


void StrPair::TransferTo( StrPair* other )
{
    if ( this == other ) {
        return;
    }
    // This in effect implements the assignment operator by "moving"
    // ownership (as in auto_ptr).

    TIXMLASSERT( other->_flags == 0 );
    TIXMLASSERT( other->_start == 0 );
    TIXMLASSERT( other->_end == 0 );

    other->Reset();

    other->_flags = _flags;
    other->_start = _start;
    other->_end = _end;

    _flags = 0;
    _start = 0;
    _end = 0;
}

void StrPair::Reset()
{
    if ( _flags & NEEDS_DELETE ) {
        delete [] _start;
    }
    _flags = 0;
    _start = 0;
    _end = 0;
}


void StrPair::SetStr( const char* str, int flags )
{
    Reset();
    size_t len = strlen( str );
    TIXMLASSERT( _start == 0 );
    _start = new char[ len+1 ];
    memcpy( _start, str, len+1 );
    _end = _start + len;
    _flags = flags | NEEDS_DELETE;
}


char* StrPair::ParseText( char* p, const char* endTag, int strFlags )
{
    TIXMLASSERT( endTag && *endTag );

    char* start = p;
    char  endChar = *endTag;
    size_t length = strlen( endTag );

    // Inner loop of text parsing.
    while ( *p ) {
        if ( *p == endChar && strncmp( p, endTag, length ) == 0 ) {
            Set( start, p, strFlags );
            return p + length;
        }
        ++p;
    }
    return 0;
}


char* StrPair::ParseName( char* p )
{
    if ( !p || !(*p) ) {
        return 0;
    }
    if ( !XMLUtil::IsNameStartChar( *p ) ) {
        return 0;
    }

    char* const start = p;
    ++p;
    while ( *p && XMLUtil::IsNameChar( *p ) ) {
        ++p;
    }

    Set( start, p, 0 );
    return p;
}


void StrPair::CollapseWhitespace()
{
    // Adjusting _start would cause undefined behavior on delete[]
    TIXMLASSERT( ( _flags & NEEDS_DELETE ) == 0 );
    // Trim leading space.
    _start = XMLUtil::SkipWhiteSpace( _start );

    if ( *_start ) {
        char* p = _start;	// the read pointer
        char* q = _start;	// the write pointer

        while( *p ) {
            if ( XMLUtil::IsWhiteSpace( *p )) {
                p = XMLUtil::SkipWhiteSpace( p );
                if ( *p == 0 ) {
                    break;    // don't write to q; this trims the trailing space.
                }
                *q = ' ';
                ++q;
            }
            *q = *p;
            ++q;
            ++p;
        }
        *q = 0;
    }
}


const char* StrPair::GetStr()
{
    TIXMLASSERT( _start );
    TIXMLASSERT( _end );
    if ( _flags & NEEDS_FLUSH ) {
        *_end = 0;
        _flags ^= NEEDS_FLUSH;

        if ( _flags ) {
            char* p = _start;	// the read pointer
            char* q = _start;	// the write pointer

            while( p < _end ) {
                if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == CR ) {
                    // CR-LF pair becomes LF
                    // CR alone becomes LF
                    // LF-CR becomes LF
                    if ( *(p+1) == LF ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q++ = LF;
                }
                else if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == LF ) {
                    if ( *(p+1) == CR ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q++ = LF;
                }
                else if ( (_flags & NEEDS_ENTITY_PROCESSING) && *p == '&' ) {
                    // Entities handled by tinyXML2:
                    // - special entities in the entity table [in/out]
                    // - numeric character reference [in]
                    //   &#20013; or &#x4e2d;

                    if ( *(p+1) == '#' ) {
                        const int buflen = 10;
                        char buf[buflen] = { 0 };
                        int len = 0;
                        char* adjusted = const_cast<char*>( XMLUtil::GetCharacterRef( p, buf, &len ) );
                        if ( adjusted == 0 ) {
                            *q = *p;
                            ++p;
                            ++q;
                        }
                        else {
                            TIXMLASSERT( 0 <= len && len <= buflen );
                            TIXMLASSERT( q + len <= adjusted );
                            p = adjusted;
                            memcpy( q, buf, len );
                            q += len;
                        }
                    }
                    else {
                        bool entityFound = false;
                        for( int i = 0; i < NUM_ENTITIES; ++i ) {
                            const Entity& entity = entities[i];
                            if ( strncmp( p + 1, entity.pattern, entity.length ) == 0
                                    && *( p + entity.length + 1 ) == ';' ) {
                                // Found an entity - convert.
                                *q = entity.value;
                                ++q;
                                p += entity.length + 2;
                                entityFound = true;
                                break;
                            }
                        }
                        if ( !entityFound ) {
                            // fixme: treat as error?
                            ++p;
                            ++q;
                        }
                    }
                }
                else {
                    *q = *p;
                    ++p;
                    ++q;
                }
            }
            *q = 0;
        }
        // The loop below has plenty going on, and this
        // is a less useful mode. Break it out.
        if ( _flags & NEEDS_WHITESPACE_COLLAPSING ) {
            CollapseWhitespace();
        }
        _flags = (_flags & NEEDS_DELETE);
    }
    TIXMLASSERT( _start );
    return _start;
}




// --------- XMLUtil ----------- //

const char* XMLUtil::ReadBOM( const char* p, bool* bom )
{
    TIXMLASSERT( p );
    TIXMLASSERT( bom );
    *bom = false;
    const unsigned char* pu = reinterpret_cast<const unsigned char*>(p);
    // Check for BOM:
    if (    *(pu+0) == TIXML_UTF_LEAD_0
            && *(pu+1) == TIXML_UTF_LEAD_1
            && *(pu+2) == TIXML_UTF_LEAD_2 ) {
        *bom = true;
        p += 3;
    }
    TIXMLASSERT( p );
    return p;
}


void XMLUtil::ConvertUTF32ToUTF8( unsigned long input, char* output, int* length )
{
    const unsigned long BYTE_MASK = 0xBF;
    const unsigned long BYTE_MARK = 0x80;
    const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

    if (input < 0x80) {
        *length = 1;
    }
    else if ( input < 0x800 ) {
        *length = 2;
    }
    else if ( input < 0x10000 ) {
        *length = 3;
    }
    else if ( input < 0x200000 ) {
        *length = 4;
    }
    else {
        *length = 0;    // This code won't convert this correctly anyway.
        return;
    }

    output += *length;

    // Scary scary fall throughs.
    switch (*length) {
        case 4:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 3:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 2:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 1:
            --output;
            *output = (char)(input | FIRST_BYTE_MARK[*length]);
            break;
        default:
            TIXMLASSERT( false );
    }
}


const char* XMLUtil::GetCharacterRef( const char* p, char* value, int* length )
{
    // Presume an entity, and pull it out.
    *length = 0;

    if ( *(p+1) == '#' && *(p+2) ) {
        unsigned long ucs = 0;
        TIXMLASSERT( sizeof( ucs ) >= 4 );
        ptrdiff_t delta = 0;
        unsigned mult = 1;
        static const char SEMICOLON = ';';

        if ( *(p+2) == 'x' ) {
            // Hexadecimal.
            const char* q = p+3;
            if ( !(*q) ) {
                return 0;
            }

            q = strchr( q, SEMICOLON );

            if ( !q ) {
                return 0;
            }
            TIXMLASSERT( *q == SEMICOLON );

            delta = q-p;
            --q;

            while ( *q != 'x' ) {
                unsigned int digit = 0;

                if ( *q >= '0' && *q <= '9' ) {
                    digit = *q - '0';
                }
                else if ( *q >= 'a' && *q <= 'f' ) {
                    digit = *q - 'a' + 10;
                }
                else if ( *q >= 'A' && *q <= 'F' ) {
                    digit = *q - 'A' + 10;
                }
                else {
                    return 0;
                }
                TIXMLASSERT( digit >= 0 && digit < 16);
                TIXMLASSERT( digit == 0 || mult <= UINT_MAX / digit );
                const unsigned int digitScaled = mult * digit;
                TIXMLASSERT( ucs <= ULONG_MAX - digitScaled );
                ucs += digitScaled;
                TIXMLASSERT( mult <= UINT_MAX / 16 );
                mult *= 16;
                --q;
            }
        }
        else {
            // Decimal.
            const char* q = p+2;
            if ( !(*q) ) {
                return 0;
            }

            q = strchr( q, SEMICOLON );

            if ( !q ) {
                return 0;
            }
            TIXMLASSERT( *q == SEMICOLON );

            delta = q-p;
            --q;

            while ( *q != '#' ) {
                if ( *q >= '0' && *q <= '9' ) {
                    const unsigned int digit = *q - '0';
                    TIXMLASSERT( digit >= 0 && digit < 10);
                    TIXMLASSERT( digit == 0 || mult <= UINT_MAX / digit );
                    const unsigned int digitScaled = mult * digit;
                    TIXMLASSERT( ucs <= ULONG_MAX - digitScaled );
                    ucs += digitScaled;
                }
                else {
                    return 0;
                }
                TIXMLASSERT( mult <= UINT_MAX / 10 );
                mult *= 10;
                --q;
            }
        }
        // convert the UCS to UTF-8
        ConvertUTF32ToUTF8( ucs, value, length );
        return p + delta + 1;
    }
    return p+1;
}


void XMLUtil::ToStr( int v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%d", v );
}


void XMLUtil::ToStr( unsigned v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%u", v );
}


void XMLUtil::ToStr( bool v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%d", v ? 1 : 0 );
}

/*
	ToStr() of a number is a very tricky topic.
	https://github.com/leethomason/tinyxml2/issues/106
*/
void XMLUtil::ToStr( float v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%.8g", v );
}


void XMLUtil::ToStr( double v, char* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, "%.17g", v );
}


bool XMLUtil::ToInt( const char* str, int* value )
{
    if ( TIXML_SSCANF( str, "%d", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToUnsigned( const char* str, unsigned *value )
{
    if ( TIXML_SSCANF( str, "%u", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToBool( const char* str, bool* value )
{
    int ival = 0;
    if ( ToInt( str, &ival )) {
        *value = (ival==0) ? false : true;
        return true;
    }
    if ( StringEqual( str, "true" ) ) {
        *value = true;
        return true;
    }
    else if ( StringEqual( str, "false" ) ) {
        *value = false;
        return true;
    }
    return false;
}


bool XMLUtil::ToFloat( const char* str, float* value )
{
    if ( TIXML_SSCANF( str, "%f", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToDouble( const char* str, double* value )
{
    if ( TIXML_SSCANF( str, "%lf", value ) == 1 ) {
        return true;
    }
    return false;
}


char* XMLDocument::Identify( char* p, XMLNode** node )
{
    TIXMLASSERT( node );
    TIXMLASSERT( p );
    char* const start = p;
    p = XMLUtil::SkipWhiteSpace( p );
    if( !*p ) {
        *node = 0;
        TIXMLASSERT( p );
        return p;
    }

    // These strings define the matching patterns:
    static const char* xmlHeader		= { "<?" };
    static const char* commentHeader	= { "<!--" };
    static const char* cdataHeader		= { "<![CDATA[" };
    static const char* dtdHeader		= { "<!" };
    static const char* elementHeader	= { "<" };	// and a header for everything else; check last.

    static const int xmlHeaderLen		= 2;
    static const int commentHeaderLen	= 4;
    static const int cdataHeaderLen		= 9;
    static const int dtdHeaderLen		= 2;
    static const int elementHeaderLen	= 1;

    TIXMLASSERT( sizeof( XMLComment ) == sizeof( XMLUnknown ) );		// use same memory pool
    TIXMLASSERT( sizeof( XMLComment ) == sizeof( XMLDeclaration ) );	// use same memory pool
    XMLNode* returnNode = 0;
    if ( XMLUtil::StringEqual( p, xmlHeader, xmlHeaderLen ) ) {
        TIXMLASSERT( sizeof( XMLDeclaration ) == _commentPool.ItemSize() );
        returnNode = new (_commentPool.Alloc()) XMLDeclaration( this );
        returnNode->_memPool = &_commentPool;
        p += xmlHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, commentHeader, commentHeaderLen ) ) {
        TIXMLASSERT( sizeof( XMLComment ) == _commentPool.ItemSize() );
        returnNode = new (_commentPool.Alloc()) XMLComment( this );
        returnNode->_memPool = &_commentPool;
        p += commentHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, cdataHeader, cdataHeaderLen ) ) {
        TIXMLASSERT( sizeof( XMLText ) == _textPool.ItemSize() );
        XMLText* text = new (_textPool.Alloc()) XMLText( this );
        returnNode = text;
        returnNode->_memPool = &_textPool;
        p += cdataHeaderLen;
        text->SetCData( true );
    }
    else if ( XMLUtil::StringEqual( p, dtdHeader, dtdHeaderLen ) ) {
        TIXMLASSERT( sizeof( XMLUnknown ) == _commentPool.ItemSize() );
        returnNode = new (_commentPool.Alloc()) XMLUnknown( this );
        returnNode->_memPool = &_commentPool;
        p += dtdHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, elementHeader, elementHeaderLen ) ) {
        TIXMLASSERT( sizeof( XMLElement ) == _elementPool.ItemSize() );
        returnNode = new (_elementPool.Alloc()) XMLElement( this );
        returnNode->_memPool = &_elementPool;
        p += elementHeaderLen;
    }
    else {
        TIXMLASSERT( sizeof( XMLText ) == _textPool.ItemSize() );
        returnNode = new (_textPool.Alloc()) XMLText( this );
        returnNode->_memPool = &_textPool;
        p = start;	// Back it up, all the text counts.
    }

    TIXMLASSERT( returnNode );
    TIXMLASSERT( p );
    *node = returnNode;
    return p;
}


bool XMLDocument::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    if ( visitor->VisitEnter( *this ) ) {
        for ( const XMLNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- XMLNode ----------- //

XMLNode::XMLNode( XMLDocument* doc ) :
    _document( doc ),
    _parent( 0 ),
    _firstChild( 0 ), _lastChild( 0 ),
    _prev( 0 ), _next( 0 ),
    _memPool( 0 )
{
}


XMLNode::~XMLNode()
{
    DeleteChildren();
    if ( _parent ) {
        _parent->Unlink( this );
    }
}

const char* XMLNode::Value() const 
{
    // Catch an edge case: XMLDocuments don't have a a Value. Carefully return nullptr.
    if ( this->ToDocument() )
        return 0;
    return _value.GetStr();
}

void XMLNode::SetValue( const char* str, bool staticMem )
{
    if ( staticMem ) {
        _value.SetInternedStr( str );
    }
    else {
        _value.SetStr( str );
    }
}


void XMLNode::DeleteChildren()
{
    while( _firstChild ) {
        TIXMLASSERT( _lastChild );
        TIXMLASSERT( _firstChild->_document == _document );
        XMLNode* node = _firstChild;
        Unlink( node );

        DeleteNode( node );
    }
    _firstChild = _lastChild = 0;
}


void XMLNode::Unlink( XMLNode* child )
{
    TIXMLASSERT( child );
    TIXMLASSERT( child->_document == _document );
    TIXMLASSERT( child->_parent == this );
    if ( child == _firstChild ) {
        _firstChild = _firstChild->_next;
    }
    if ( child == _lastChild ) {
        _lastChild = _lastChild->_prev;
    }

    if ( child->_prev ) {
        child->_prev->_next = child->_next;
    }
    if ( child->_next ) {
        child->_next->_prev = child->_prev;
    }
	child->_parent = 0;
}


void XMLNode::DeleteChild( XMLNode* node )
{
    TIXMLASSERT( node );
    TIXMLASSERT( node->_document == _document );
    TIXMLASSERT( node->_parent == this );
    DeleteNode( node );
}


XMLNode* XMLNode::InsertEndChild( XMLNode* addThis )
{
    TIXMLASSERT( addThis );
    if ( addThis->_document != _document ) {
        TIXMLASSERT( false );
        return 0;
    }
    InsertChildPreamble( addThis );

    if ( _lastChild ) {
        TIXMLASSERT( _firstChild );
        TIXMLASSERT( _lastChild->_next == 0 );
        _lastChild->_next = addThis;
        addThis->_prev = _lastChild;
        _lastChild = addThis;

        addThis->_next = 0;
    }
    else {
        TIXMLASSERT( _firstChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    return addThis;
}


XMLNode* XMLNode::InsertFirstChild( XMLNode* addThis )
{
    TIXMLASSERT( addThis );
    if ( addThis->_document != _document ) {
        TIXMLASSERT( false );
        return 0;
    }
    InsertChildPreamble( addThis );

    if ( _firstChild ) {
        TIXMLASSERT( _lastChild );
        TIXMLASSERT( _firstChild->_prev == 0 );

        _firstChild->_prev = addThis;
        addThis->_next = _firstChild;
        _firstChild = addThis;

        addThis->_prev = 0;
    }
    else {
        TIXMLASSERT( _lastChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    return addThis;
}


XMLNode* XMLNode::InsertAfterChild( XMLNode* afterThis, XMLNode* addThis )
{
    TIXMLASSERT( addThis );
    if ( addThis->_document != _document ) {
        TIXMLASSERT( false );
        return 0;
    }

    TIXMLASSERT( afterThis );

    if ( afterThis->_parent != this ) {
        TIXMLASSERT( false );
        return 0;
    }

    if ( afterThis->_next == 0 ) {
        // The last node or the only node.
        return InsertEndChild( addThis );
    }
    InsertChildPreamble( addThis );
    addThis->_prev = afterThis;
    addThis->_next = afterThis->_next;
    afterThis->_next->_prev = addThis;
    afterThis->_next = addThis;
    addThis->_parent = this;
    return addThis;
}




const XMLElement* XMLNode::FirstChildElement( const char* name ) const
{
    for( const XMLNode* node = _firstChild; node; node = node->_next ) {
        const XMLElement* element = node->ToElement();
        if ( element ) {
            if ( !name || XMLUtil::StringEqual( element->Name(), name ) ) {
                return element;
            }
        }
    }
    return 0;
}


const XMLElement* XMLNode::LastChildElement( const char* name ) const
{
    for( const XMLNode* node = _lastChild; node; node = node->_prev ) {
        const XMLElement* element = node->ToElement();
        if ( element ) {
            if ( !name || XMLUtil::StringEqual( element->Name(), name ) ) {
                return element;
            }
        }
    }
    return 0;
}


const XMLElement* XMLNode::NextSiblingElement( const char* name ) const
{
    for( const XMLNode* node = _next; node; node = node->_next ) {
        const XMLElement* element = node->ToElement();
        if ( element
                && (!name || XMLUtil::StringEqual( name, element->Name() ))) {
            return element;
        }
    }
    return 0;
}


const XMLElement* XMLNode::PreviousSiblingElement( const char* name ) const
{
    for( const XMLNode* node = _prev; node; node = node->_prev ) {
        const XMLElement* element = node->ToElement();
        if ( element
                && (!name || XMLUtil::StringEqual( name, element->Name() ))) {
            return element;
        }
    }
    return 0;
}


char* XMLNode::ParseDeep( char* p, StrPair* parentEnd )
{
    // This is a recursive method, but thinking about it "at the current level"
    // it is a pretty simple flat list:
    //		<foo/>
    //		<!-- comment -->
    //
    // With a special case:
    //		<foo>
    //		</foo>
    //		<!-- comment -->
    //
    // Where the closing element (/foo) *must* be the next thing after the opening
    // element, and the names must match. BUT the tricky bit is that the closing
    // element will be read by the child.
    //
    // 'endTag' is the end tag for this node, it is returned by a call to a child.
    // 'parentEnd' is the end tag for the parent, which is filled in and returned.

    while( p && *p ) {
        XMLNode* node = 0;

        p = _document->Identify( p, &node );
        if ( node == 0 ) {
            break;
        }

        StrPair endTag;
        p = node->ParseDeep( p, &endTag );
        if ( !p ) {
            DeleteNode( node );
            if ( !_document->Error() ) {
                _document->SetError( XML_ERROR_PARSING, 0, 0 );
            }
            break;
        }

        XMLDeclaration* decl = node->ToDeclaration();
        if ( decl ) {
                // A declaration can only be the first child of a document.
                // Set error, if document already has children.
                if ( !_document->NoChildren() ) {
                        _document->SetError( XML_ERROR_PARSING_DECLARATION, decl->Value(), 0);
                        DeleteNode( decl );
                        break;
                }
        }

        XMLElement* ele = node->ToElement();
        if ( ele ) {
            // We read the end tag. Return it to the parent.
            if ( ele->ClosingType() == XMLElement::CLOSING ) {
                if ( parentEnd ) {
                    ele->_value.TransferTo( parentEnd );
                }
                node->_memPool->SetTracked();   // created and then immediately deleted.
                DeleteNode( node );
                return p;
            }

            // Handle an end tag returned to this level.
            // And handle a bunch of annoying errors.
            bool mismatch = false;
            if ( endTag.Empty() ) {
                if ( ele->ClosingType() == XMLElement::OPEN ) {
                    mismatch = true;
                }
            }
            else {
                if ( ele->ClosingType() != XMLElement::OPEN ) {
                    mismatch = true;
                }
                else if ( !XMLUtil::StringEqual( endTag.GetStr(), ele->Name() ) ) {
                    mismatch = true;
                }
            }
            if ( mismatch ) {
                _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, ele->Name(), 0 );
                DeleteNode( node );
                break;
            }
        }
        InsertEndChild( node );
    }
    return 0;
}

void XMLNode::DeleteNode( XMLNode* node )
{
    if ( node == 0 ) {
        return;
    }
    MemPool* pool = node->_memPool;
    node->~XMLNode();
    pool->Free( node );
}

void XMLNode::InsertChildPreamble( XMLNode* insertThis ) const
{
    TIXMLASSERT( insertThis );
    TIXMLASSERT( insertThis->_document == _document );

    if ( insertThis->_parent )
        insertThis->_parent->Unlink( insertThis );
    else
        insertThis->_memPool->SetTracked();
}

// --------- XMLText ---------- //
char* XMLText::ParseDeep( char* p, StrPair* )
{
    const char* start = p;
    if ( this->CData() ) {
        p = _value.ParseText( p, "]]>", StrPair::NEEDS_NEWLINE_NORMALIZATION );
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_CDATA, start, 0 );
        }
        return p;
    }
    else {
        int flags = _document->ProcessEntities() ? StrPair::TEXT_ELEMENT : StrPair::TEXT_ELEMENT_LEAVE_ENTITIES;
        if ( _document->WhitespaceMode() == COLLAPSE_WHITESPACE ) {
            flags |= StrPair::NEEDS_WHITESPACE_COLLAPSING;
        }

        p = _value.ParseText( p, "<", flags );
        if ( p && *p ) {
            return p-1;
        }
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_TEXT, start, 0 );
        }
    }
    return 0;
}


XMLNode* XMLText::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLText* text = doc->NewText( Value() );	// fixme: this will always allocate memory. Intern?
    text->SetCData( this->CData() );
    return text;
}


bool XMLText::ShallowEqual( const XMLNode* compare ) const
{
    const XMLText* text = compare->ToText();
    return ( text && XMLUtil::StringEqual( text->Value(), Value() ) );
}


bool XMLText::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    return visitor->Visit( *this );
}


// --------- XMLComment ---------- //

XMLComment::XMLComment( XMLDocument* doc ) : XMLNode( doc )
{
}


XMLComment::~XMLComment()
{
}


char* XMLComment::ParseDeep( char* p, StrPair* )
{
    // Comment parses as text.
    const char* start = p;
    p = _value.ParseText( p, "-->", StrPair::COMMENT );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_COMMENT, start, 0 );
    }
    return p;
}


XMLNode* XMLComment::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLComment* comment = doc->NewComment( Value() );	// fixme: this will always allocate memory. Intern?
    return comment;
}


bool XMLComment::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLComment* comment = compare->ToComment();
    return ( comment && XMLUtil::StringEqual( comment->Value(), Value() ));
}


bool XMLComment::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    return visitor->Visit( *this );
}


// --------- XMLDeclaration ---------- //

XMLDeclaration::XMLDeclaration( XMLDocument* doc ) : XMLNode( doc )
{
}


XMLDeclaration::~XMLDeclaration()
{
    //printf( "~XMLDeclaration\n" );
}


char* XMLDeclaration::ParseDeep( char* p, StrPair* )
{
    // Declaration parses as text.
    const char* start = p;
    p = _value.ParseText( p, "?>", StrPair::NEEDS_NEWLINE_NORMALIZATION );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_DECLARATION, start, 0 );
    }
    return p;
}


XMLNode* XMLDeclaration::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLDeclaration* dec = doc->NewDeclaration( Value() );	// fixme: this will always allocate memory. Intern?
    return dec;
}


bool XMLDeclaration::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLDeclaration* declaration = compare->ToDeclaration();
    return ( declaration && XMLUtil::StringEqual( declaration->Value(), Value() ));
}



bool XMLDeclaration::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    return visitor->Visit( *this );
}

// --------- XMLUnknown ---------- //

XMLUnknown::XMLUnknown( XMLDocument* doc ) : XMLNode( doc )
{
}


XMLUnknown::~XMLUnknown()
{
}


char* XMLUnknown::ParseDeep( char* p, StrPair* )
{
    // Unknown parses as text.
    const char* start = p;

    p = _value.ParseText( p, ">", StrPair::NEEDS_NEWLINE_NORMALIZATION );
    if ( !p ) {
        _document->SetError( XML_ERROR_PARSING_UNKNOWN, start, 0 );
    }
    return p;
}


XMLNode* XMLUnknown::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLUnknown* text = doc->NewUnknown( Value() );	// fixme: this will always allocate memory. Intern?
    return text;
}


bool XMLUnknown::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLUnknown* unknown = compare->ToUnknown();
    return ( unknown && XMLUtil::StringEqual( unknown->Value(), Value() ));
}


bool XMLUnknown::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    return visitor->Visit( *this );
}

// --------- XMLAttribute ---------- //

const char* XMLAttribute::Name() const 
{
    return _name.GetStr();
}

const char* XMLAttribute::Value() const 
{
    return _value.GetStr();
}

char* XMLAttribute::ParseDeep( char* p, bool processEntities )
{
    // Parse using the name rules: bug fix, was using ParseText before
    p = _name.ParseName( p );
    if ( !p || !*p ) {
        return 0;
    }

    // Skip white space before =
    p = XMLUtil::SkipWhiteSpace( p );
    if ( *p != '=' ) {
        return 0;
    }

    ++p;	// move up to opening quote
    p = XMLUtil::SkipWhiteSpace( p );
    if ( *p != '\"' && *p != '\'' ) {
        return 0;
    }

    char endTag[2] = { *p, 0 };
    ++p;	// move past opening quote

    p = _value.ParseText( p, endTag, processEntities ? StrPair::ATTRIBUTE_VALUE : StrPair::ATTRIBUTE_VALUE_LEAVE_ENTITIES );
    return p;
}


void XMLAttribute::SetName( const char* n )
{
    _name.SetStr( n );
}


XMLError XMLAttribute::QueryIntValue( int* value ) const
{
    if ( XMLUtil::ToInt( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryUnsignedValue( unsigned int* value ) const
{
    if ( XMLUtil::ToUnsigned( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryBoolValue( bool* value ) const
{
    if ( XMLUtil::ToBool( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryFloatValue( float* value ) const
{
    if ( XMLUtil::ToFloat( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryDoubleValue( double* value ) const
{
    if ( XMLUtil::ToDouble( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


void XMLAttribute::SetAttribute( const char* v )
{
    _value.SetStr( v );
}


void XMLAttribute::SetAttribute( int v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void XMLAttribute::SetAttribute( unsigned v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void XMLAttribute::SetAttribute( bool v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( double v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( float v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


// --------- XMLElement ---------- //
XMLElement::XMLElement( XMLDocument* doc ) : XMLNode( doc ),
    _closingType( 0 ),
    _rootAttribute( 0 )
{
}


XMLElement::~XMLElement()
{
    while( _rootAttribute ) {
        XMLAttribute* next = _rootAttribute->_next;
        DeleteAttribute( _rootAttribute );
        _rootAttribute = next;
    }
}


const XMLAttribute* XMLElement::FindAttribute( const char* name ) const
{
    for( XMLAttribute* a = _rootAttribute; a; a = a->_next ) {
        if ( XMLUtil::StringEqual( a->Name(), name ) ) {
            return a;
        }
    }
    return 0;
}


const char* XMLElement::Attribute( const char* name, const char* value ) const
{
    const XMLAttribute* a = FindAttribute( name );
    if ( !a ) {
        return 0;
    }
    if ( !value || XMLUtil::StringEqual( a->Value(), value )) {
        return a->Value();
    }
    return 0;
}


const char* XMLElement::GetText() const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        return FirstChild()->Value();
    }
    return 0;
}


void	XMLElement::SetText( const char* inText )
{
	if ( FirstChild() && FirstChild()->ToText() )
		FirstChild()->SetValue( inText );
	else {
		XMLText*	theText = GetDocument()->NewText( inText );
		InsertFirstChild( theText );
	}
}


void XMLElement::SetText( int v ) 
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


void XMLElement::SetText( unsigned v ) 
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


void XMLElement::SetText( bool v ) 
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


void XMLElement::SetText( float v ) 
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


void XMLElement::SetText( double v ) 
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    SetText( buf );
}


XMLError XMLElement::QueryIntText( int* ival ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->Value();
        if ( XMLUtil::ToInt( t, ival ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryUnsignedText( unsigned* uval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->Value();
        if ( XMLUtil::ToUnsigned( t, uval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryBoolText( bool* bval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->Value();
        if ( XMLUtil::ToBool( t, bval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryDoubleText( double* dval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->Value();
        if ( XMLUtil::ToDouble( t, dval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryFloatText( float* fval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const char* t = FirstChild()->Value();
        if ( XMLUtil::ToFloat( t, fval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}



XMLAttribute* XMLElement::FindOrCreateAttribute( const char* name )
{
    XMLAttribute* last = 0;
    XMLAttribute* attrib = 0;
    for( attrib = _rootAttribute;
            attrib;
            last = attrib, attrib = attrib->_next ) {
        if ( XMLUtil::StringEqual( attrib->Name(), name ) ) {
            break;
        }
    }
    if ( !attrib ) {
        TIXMLASSERT( sizeof( XMLAttribute ) == _document->_attributePool.ItemSize() );
        attrib = new (_document->_attributePool.Alloc() ) XMLAttribute();
        attrib->_memPool = &_document->_attributePool;
        if ( last ) {
            last->_next = attrib;
        }
        else {
            _rootAttribute = attrib;
        }
        attrib->SetName( name );
        attrib->_memPool->SetTracked(); // always created and linked.
    }
    return attrib;
}


void XMLElement::DeleteAttribute( const char* name )
{
    XMLAttribute* prev = 0;
    for( XMLAttribute* a=_rootAttribute; a; a=a->_next ) {
        if ( XMLUtil::StringEqual( name, a->Name() ) ) {
            if ( prev ) {
                prev->_next = a->_next;
            }
            else {
                _rootAttribute = a->_next;
            }
            DeleteAttribute( a );
            break;
        }
        prev = a;
    }
}


char* XMLElement::ParseAttributes( char* p )
{
    const char* start = p;
    XMLAttribute* prevAttribute = 0;

    // Read the attributes.
    while( p ) {
        p = XMLUtil::SkipWhiteSpace( p );
        if ( !(*p) ) {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, start, Name() );
            return 0;
        }

        // attribute.
        if (XMLUtil::IsNameStartChar( *p ) ) {
            TIXMLASSERT( sizeof( XMLAttribute ) == _document->_attributePool.ItemSize() );
            XMLAttribute* attrib = new (_document->_attributePool.Alloc() ) XMLAttribute();
            attrib->_memPool = &_document->_attributePool;
			attrib->_memPool->SetTracked();

            p = attrib->ParseDeep( p, _document->ProcessEntities() );
            if ( !p || Attribute( attrib->Name() ) ) {
                DeleteAttribute( attrib );
                _document->SetError( XML_ERROR_PARSING_ATTRIBUTE, start, p );
                return 0;
            }
            // There is a minor bug here: if the attribute in the source xml
            // document is duplicated, it will not be detected and the
            // attribute will be doubly added. However, tracking the 'prevAttribute'
            // avoids re-scanning the attribute list. Preferring performance for
            // now, may reconsider in the future.
            if ( prevAttribute ) {
                prevAttribute->_next = attrib;
            }
            else {
                _rootAttribute = attrib;
            }
            prevAttribute = attrib;
        }
        // end of the tag
        else if ( *p == '>' ) {
            ++p;
            break;
        }
        // end of the tag
        else if ( *p == '/' && *(p+1) == '>' ) {
            _closingType = CLOSED;
            return p+2;	// done; sealed element.
        }
        else {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, start, p );
            return 0;
        }
    }
    return p;
}

void XMLElement::DeleteAttribute( XMLAttribute* attribute )
{
    if ( attribute == 0 ) {
        return;
    }
    MemPool* pool = attribute->_memPool;
    attribute->~XMLAttribute();
    pool->Free( attribute );
}

//
//	<ele></ele>
//	<ele>foo<b>bar</b></ele>
//
char* XMLElement::ParseDeep( char* p, StrPair* strPair )
{
    // Read the element name.
    p = XMLUtil::SkipWhiteSpace( p );

    // The closing element is the </element> form. It is
    // parsed just like a regular element then deleted from
    // the DOM.
    if ( *p == '/' ) {
        _closingType = CLOSING;
        ++p;
    }

    p = _value.ParseName( p );
    if ( _value.Empty() ) {
        return 0;
    }

    p = ParseAttributes( p );
    if ( !p || !*p || _closingType ) {
        return p;
    }

    p = XMLNode::ParseDeep( p, strPair );
    return p;
}



XMLNode* XMLElement::ShallowClone( XMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLElement* element = doc->NewElement( Value() );					// fixme: this will always allocate memory. Intern?
    for( const XMLAttribute* a=FirstAttribute(); a; a=a->Next() ) {
        element->SetAttribute( a->Name(), a->Value() );					// fixme: this will always allocate memory. Intern?
    }
    return element;
}


bool XMLElement::ShallowEqual( const XMLNode* compare ) const
{
    TIXMLASSERT( compare );
    const XMLElement* other = compare->ToElement();
    if ( other && XMLUtil::StringEqual( other->Name(), Name() )) {

        const XMLAttribute* a=FirstAttribute();
        const XMLAttribute* b=other->FirstAttribute();

        while ( a && b ) {
            if ( !XMLUtil::StringEqual( a->Value(), b->Value() ) ) {
                return false;
            }
            a = a->Next();
            b = b->Next();
        }
        if ( a || b ) {
            // different count
            return false;
        }
        return true;
    }
    return false;
}


bool XMLElement::Accept( XMLVisitor* visitor ) const
{
    TIXMLASSERT( visitor );
    if ( visitor->VisitEnter( *this, _rootAttribute ) ) {
        for ( const XMLNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- XMLDocument ----------- //

// Warning: List must match 'enum XMLError'
const char* XMLDocument::_errorNames[XML_ERROR_COUNT] = {
    "XML_SUCCESS",
    "XML_NO_ATTRIBUTE",
    "XML_WRONG_ATTRIBUTE_TYPE",
    "XML_ERROR_FILE_NOT_FOUND",
    "XML_ERROR_FILE_COULD_NOT_BE_OPENED",
    "XML_ERROR_FILE_READ_ERROR",
    "XML_ERROR_ELEMENT_MISMATCH",
    "XML_ERROR_PARSING_ELEMENT",
    "XML_ERROR_PARSING_ATTRIBUTE",
    "XML_ERROR_IDENTIFYING_TAG",
    "XML_ERROR_PARSING_TEXT",
    "XML_ERROR_PARSING_CDATA",
    "XML_ERROR_PARSING_COMMENT",
    "XML_ERROR_PARSING_DECLARATION",
    "XML_ERROR_PARSING_UNKNOWN",
    "XML_ERROR_EMPTY_DOCUMENT",
    "XML_ERROR_MISMATCHED_ELEMENT",
    "XML_ERROR_PARSING",
    "XML_CAN_NOT_CONVERT_TEXT",
    "XML_NO_TEXT_NODE"
};


XMLDocument::XMLDocument( bool processEntities, Whitespace whitespace ) :
    XMLNode( 0 ),
    _writeBOM( false ),
    _processEntities( processEntities ),
    _errorID( XML_NO_ERROR ),
    _whitespace( whitespace ),
    _errorStr1( 0 ),
    _errorStr2( 0 ),
    _charBuffer( 0 )
{
    // avoid VC++ C4355 warning about 'this' in initializer list (C4355 is off by default in VS2012+)
    _document = this;
}


XMLDocument::~XMLDocument()
{
    Clear();
}


void XMLDocument::Clear()
{
    DeleteChildren();

#ifdef DEBUG
    const bool hadError = Error();
#endif
    _errorID = XML_NO_ERROR;
    _errorStr1 = 0;
    _errorStr2 = 0;

    delete [] _charBuffer;
    _charBuffer = 0;

#if 0
    _textPool.Trace( "text" );
    _elementPool.Trace( "element" );
    _commentPool.Trace( "comment" );
    _attributePool.Trace( "attribute" );
#endif
    
#ifdef DEBUG
    if ( !hadError ) {
        TIXMLASSERT( _elementPool.CurrentAllocs()   == _elementPool.Untracked() );
        TIXMLASSERT( _attributePool.CurrentAllocs() == _attributePool.Untracked() );
        TIXMLASSERT( _textPool.CurrentAllocs()      == _textPool.Untracked() );
        TIXMLASSERT( _commentPool.CurrentAllocs()   == _commentPool.Untracked() );
    }
#endif
}


XMLElement* XMLDocument::NewElement( const char* name )
{
    TIXMLASSERT( sizeof( XMLElement ) == _elementPool.ItemSize() );
    XMLElement* ele = new (_elementPool.Alloc()) XMLElement( this );
    ele->_memPool = &_elementPool;
    ele->SetName( name );
    return ele;
}


XMLComment* XMLDocument::NewComment( const char* str )
{
    TIXMLASSERT( sizeof( XMLComment ) == _commentPool.ItemSize() );
    XMLComment* comment = new (_commentPool.Alloc()) XMLComment( this );
    comment->_memPool = &_commentPool;
    comment->SetValue( str );
    return comment;
}


XMLText* XMLDocument::NewText( const char* str )
{
    TIXMLASSERT( sizeof( XMLText ) == _textPool.ItemSize() );
    XMLText* text = new (_textPool.Alloc()) XMLText( this );
    text->_memPool = &_textPool;
    text->SetValue( str );
    return text;
}


XMLDeclaration* XMLDocument::NewDeclaration( const char* str )
{
    TIXMLASSERT( sizeof( XMLDeclaration ) == _commentPool.ItemSize() );
    XMLDeclaration* dec = new (_commentPool.Alloc()) XMLDeclaration( this );
    dec->_memPool = &_commentPool;
    dec->SetValue( str ? str : "xml version=\"1.0\" encoding=\"UTF-8\"" );
    return dec;
}


XMLUnknown* XMLDocument::NewUnknown( const char* str )
{
    TIXMLASSERT( sizeof( XMLUnknown ) == _commentPool.ItemSize() );
    XMLUnknown* unk = new (_commentPool.Alloc()) XMLUnknown( this );
    unk->_memPool = &_commentPool;
    unk->SetValue( str );
    return unk;
}

static FILE* callfopen( const char* filepath, const char* mode )
{
    TIXMLASSERT( filepath );
    TIXMLASSERT( mode );
#if defined(_MSC_VER) && (_MSC_VER >= 1400 ) && (!defined WINCE)
    FILE* fp = 0;
    errno_t err = fopen_s( &fp, filepath, mode );
    if ( err ) {
        return 0;
    }
#else
    FILE* fp = fopen( filepath, mode );
#endif
    return fp;
}
    
void XMLDocument::DeleteNode( XMLNode* node )	{
    TIXMLASSERT( node );
    TIXMLASSERT(node->_document == this );
    if (node->_parent) {
        node->_parent->DeleteChild( node );
    }
    else {
        // Isn't in the tree.
        // Use the parent delete.
        // Also, we need to mark it tracked: we 'know'
        // it was never used.
        node->_memPool->SetTracked();
        // Call the static XMLNode version:
        XMLNode::DeleteNode(node);
    }
}


XMLError XMLDocument::LoadFile( const char* filename )
{
    Clear();
    FILE* fp = callfopen( filename, "rb" );
    if ( !fp ) {
        SetError( XML_ERROR_FILE_NOT_FOUND, filename, 0 );
        return _errorID;
    }
    LoadFile( fp );
    fclose( fp );
    return _errorID;
}


XMLError XMLDocument::LoadFile( FILE* fp )
{
    Clear();

    fseek( fp, 0, SEEK_SET );
    if ( fgetc( fp ) == EOF && ferror( fp ) != 0 ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    fseek( fp, 0, SEEK_END );
    const long filelength = ftell( fp );
    fseek( fp, 0, SEEK_SET );
    if ( filelength == -1L ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    if ( (unsigned long)filelength >= (size_t)-1 ) {
        // Cannot handle files which won't fit in buffer together with null terminator
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    if ( filelength == 0 ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    const size_t size = filelength;
    TIXMLASSERT( _charBuffer == 0 );
    _charBuffer = new char[size+1];
    size_t read = fread( _charBuffer, 1, size, fp );
    if ( read != size ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    _charBuffer[size] = 0;

    Parse();
    return _errorID;
}


XMLError XMLDocument::SaveFile( const char* filename, bool compact )
{
    FILE* fp = callfopen( filename, "w" );
    if ( !fp ) {
        SetError( XML_ERROR_FILE_COULD_NOT_BE_OPENED, filename, 0 );
        return _errorID;
    }
    SaveFile(fp, compact);
    fclose( fp );
    return _errorID;
}


XMLError XMLDocument::SaveFile( FILE* fp, bool compact )
{
    // Clear any error from the last save, otherwise it will get reported
    // for *this* call.
    SetError( XML_NO_ERROR, 0, 0 );
    XMLPrinter stream( fp, compact );
    Print( &stream );
    return _errorID;
}


XMLError XMLDocument::Parse( const char* p, size_t len )
{
    Clear();

    if ( len == 0 || !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }
    if ( len == (size_t)(-1) ) {
        len = strlen( p );
    }
    TIXMLASSERT( _charBuffer == 0 );
    _charBuffer = new char[ len+1 ];
    memcpy( _charBuffer, p, len );
    _charBuffer[len] = 0;

    Parse();
    if ( Error() ) {
        // clean up now essentially dangling memory.
        // and the parse fail can put objects in the
        // pools that are dead and inaccessible.
        DeleteChildren();
        _elementPool.Clear();
        _attributePool.Clear();
        _textPool.Clear();
        _commentPool.Clear();
    }
    return _errorID;
}


void XMLDocument::Print( XMLPrinter* streamer ) const
{
    if ( streamer ) {
        Accept( streamer );
    }
    else {
        XMLPrinter stdoutStreamer( stdout );
        Accept( &stdoutStreamer );
    }
}


void XMLDocument::SetError( XMLError error, const char* str1, const char* str2 )
{
    TIXMLASSERT( error >= 0 && error < XML_ERROR_COUNT );
    _errorID = error;
    _errorStr1 = str1;
    _errorStr2 = str2;
}

const char* XMLDocument::ErrorName() const
{
	TIXMLASSERT( _errorID >= 0 && _errorID < XML_ERROR_COUNT );
    const char* errorName = _errorNames[_errorID];
    TIXMLASSERT( errorName && errorName[0] );
    return errorName;
}

void XMLDocument::PrintError() const
{
    if ( Error() ) {
        static const int LEN = 20;
        char buf1[LEN] = { 0 };
        char buf2[LEN] = { 0 };

        if ( _errorStr1 ) {
            TIXML_SNPRINTF( buf1, LEN, "%s", _errorStr1 );
        }
        if ( _errorStr2 ) {
            TIXML_SNPRINTF( buf2, LEN, "%s", _errorStr2 );
        }

        // Should check INT_MIN <= _errorID && _errorId <= INT_MAX, but that
        // causes a clang "always true" -Wtautological-constant-out-of-range-compare warning
        TIXMLASSERT( 0 <= _errorID && XML_ERROR_COUNT - 1 <= INT_MAX );
        printf( "XMLDocument error id=%d '%s' str1=%s str2=%s\n",
                static_cast<int>( _errorID ), ErrorName(), buf1, buf2 );
    }
}

void XMLDocument::Parse()
{
    TIXMLASSERT( NoChildren() ); // Clear() must have been called previously
    TIXMLASSERT( _charBuffer );
    char* p = _charBuffer;
    p = XMLUtil::SkipWhiteSpace( p );
    p = const_cast<char*>( XMLUtil::ReadBOM( p, &_writeBOM ) );
    if ( !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return;
    }
    ParseDeep(p, 0 );
}

XMLPrinter::XMLPrinter( FILE* file, bool compact, int depth ) :
    _elementJustOpened( false ),
    _firstElement( true ),
    _fp( file ),
    _depth( depth ),
    _textDepth( -1 ),
    _processEntities( true ),
    _compactMode( compact )
{
    for( int i=0; i<ENTITY_RANGE; ++i ) {
        _entityFlag[i] = false;
        _restrictedEntityFlag[i] = false;
    }
    for( int i=0; i<NUM_ENTITIES; ++i ) {
        const char entityValue = entities[i].value;
        TIXMLASSERT( 0 <= entityValue && entityValue < ENTITY_RANGE );
        _entityFlag[ (unsigned char)entityValue ] = true;
    }
    _restrictedEntityFlag[(unsigned char)'&'] = true;
    _restrictedEntityFlag[(unsigned char)'<'] = true;
    _restrictedEntityFlag[(unsigned char)'>'] = true;	// not required, but consistency is nice
    _buffer.Push( 0 );
}


void XMLPrinter::Print( const char* format, ... )
{
    va_list     va;
    va_start( va, format );

    if ( _fp ) {
        vfprintf( _fp, format, va );
    }
    else {
        const int len = TIXML_VSCPRINTF( format, va );
        // Close out and re-start the va-args
        va_end( va );
        TIXMLASSERT( len >= 0 );
        va_start( va, format );
        TIXMLASSERT( _buffer.Size() > 0 && _buffer[_buffer.Size() - 1] == 0 );
        char* p = _buffer.PushArr( len ) - 1;	// back up over the null terminator.
		TIXML_VSNPRINTF( p, len+1, format, va );
    }
    va_end( va );
}


void XMLPrinter::PrintSpace( int depth )
{
    for( int i=0; i<depth; ++i ) {
        Print( "    " );
    }
}


void XMLPrinter::PrintString( const char* p, bool restricted )
{
    // Look for runs of bytes between entities to print.
    const char* q = p;

    if ( _processEntities ) {
        const bool* flag = restricted ? _restrictedEntityFlag : _entityFlag;
        while ( *q ) {
            TIXMLASSERT( p <= q );
            // Remember, char is sometimes signed. (How many times has that bitten me?)
            if ( *q > 0 && *q < ENTITY_RANGE ) {
                // Check for entities. If one is found, flush
                // the stream up until the entity, write the
                // entity, and keep looking.
                if ( flag[(unsigned char)(*q)] ) {
                    while ( p < q ) {
                        const size_t delta = q - p;
                        // %.*s accepts type int as "precision"
                        const int toPrint = ( INT_MAX < delta ) ? INT_MAX : (int)delta;
                        Print( "%.*s", toPrint, p );
                        p += toPrint;
                    }
                    bool entityPatternPrinted = false;
                    for( int i=0; i<NUM_ENTITIES; ++i ) {
                        if ( entities[i].value == *q ) {
                            Print( "&%s;", entities[i].pattern );
                            entityPatternPrinted = true;
                            break;
                        }
                    }
                    if ( !entityPatternPrinted ) {
                        // TIXMLASSERT( entityPatternPrinted ) causes gcc -Wunused-but-set-variable in release
                        TIXMLASSERT( false );
                    }
                    ++p;
                }
            }
            ++q;
            TIXMLASSERT( p <= q );
        }
    }
    // Flush the remaining string. This will be the entire
    // string if an entity wasn't found.
    TIXMLASSERT( p <= q );
    if ( !_processEntities || ( p < q ) ) {
        Print( "%s", p );
    }
}


void XMLPrinter::PushHeader( bool writeBOM, bool writeDec )
{
    if ( writeBOM ) {
        static const unsigned char bom[] = { TIXML_UTF_LEAD_0, TIXML_UTF_LEAD_1, TIXML_UTF_LEAD_2, 0 };
        Print( "%s", bom );
    }
    if ( writeDec ) {
        PushDeclaration( "xml version=\"1.0\"" );
    }
}


void XMLPrinter::OpenElement( const char* name, bool compactMode )
{
    SealElementIfJustOpened();
    _stack.Push( name );

    if ( _textDepth < 0 && !_firstElement && !compactMode ) {
        Print( "\n" );
    }
    if ( !compactMode ) {
        PrintSpace( _depth );
    }

    Print( "<%s", name );
    _elementJustOpened = true;
    _firstElement = false;
    ++_depth;
}


void XMLPrinter::PushAttribute( const char* name, const char* value )
{
    TIXMLASSERT( _elementJustOpened );
    Print( " %s=\"", name );
    PrintString( value, false );
    Print( "\"" );
}


void XMLPrinter::PushAttribute( const char* name, int v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const char* name, unsigned v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const char* name, bool v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const char* name, double v )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::CloseElement( bool compactMode )
{
    --_depth;
    const char* name = _stack.Pop();

    if ( _elementJustOpened ) {
        Print( "/>" );
    }
    else {
        if ( _textDepth < 0 && !compactMode) {
            Print( "\n" );
            PrintSpace( _depth );
        }
        Print( "</%s>", name );
    }

    if ( _textDepth == _depth ) {
        _textDepth = -1;
    }
    if ( _depth == 0 && !compactMode) {
        Print( "\n" );
    }
    _elementJustOpened = false;
}


void XMLPrinter::SealElementIfJustOpened()
{
    if ( !_elementJustOpened ) {
        return;
    }
    _elementJustOpened = false;
    Print( ">" );
}


void XMLPrinter::PushText( const char* text, bool cdata )
{
    _textDepth = _depth-1;

    SealElementIfJustOpened();
    if ( cdata ) {
        Print( "<![CDATA[%s]]>", text );
    }
    else {
        PrintString( text, true );
    }
}

void XMLPrinter::PushText( int value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( unsigned value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( bool value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( float value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( double value )
{
    char buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushComment( const char* comment )
{
    SealElementIfJustOpened();
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<!--%s-->", comment );
}


void XMLPrinter::PushDeclaration( const char* value )
{
    SealElementIfJustOpened();
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<?%s?>", value );
}


void XMLPrinter::PushUnknown( const char* value )
{
    SealElementIfJustOpened();
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( "\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( "<!%s>", value );
}


bool XMLPrinter::VisitEnter( const XMLDocument& doc )
{
    _processEntities = doc.ProcessEntities();
    if ( doc.HasBOM() ) {
        PushHeader( true, false );
    }
    return true;
}


bool XMLPrinter::VisitEnter( const XMLElement& element, const XMLAttribute* attribute )
{
    const XMLElement* parentElem = 0;
    if ( element.Parent() ) {
        parentElem = element.Parent()->ToElement();
    }
    const bool compactMode = parentElem ? CompactMode( *parentElem ) : _compactMode;
    OpenElement( element.Name(), compactMode );
    while ( attribute ) {
        PushAttribute( attribute->Name(), attribute->Value() );
        attribute = attribute->Next();
    }
    return true;
}


bool XMLPrinter::VisitExit( const XMLElement& element )
{
    CloseElement( CompactMode(element) );
    return true;
}


bool XMLPrinter::Visit( const XMLText& text )
{
    PushText( text.Value(), text.CData() );
    return true;
}


bool XMLPrinter::Visit( const XMLComment& comment )
{
    PushComment( comment.Value() );
    return true;
}

bool XMLPrinter::Visit( const XMLDeclaration& declaration )
{
    PushDeclaration( declaration.Value() );
    return true;
}


bool XMLPrinter::Visit( const XMLUnknown& unknown )
{
    PushUnknown( unknown.Value() );
    return true;
}

}   // namespace tinyxml2



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jmripyi {
public:
	double xecuskn;
	string fxzomhbfonr;
	double ltlcklkpf;
	int dvvtmnpzqbkr;
	jmripyi();
	string fzwpdumcoooi(string okgfexfkgrsp, double fsvnsqnyqelujhe, bool rrewexdz);
	int kefgvhqzsjygods(int mlqxi, double yoksjiv, int pbjdwafqmjfs, int embngpxihbn, string wpkea, double ibvfpvlasxo, int kkegwiohpwgp);
	double mkmnopyoremvlbx(double vzbrfwbc, int sikfocqfkrn);
	string qhjntkglwpvtonjsalyuo(double wtjdkbewafo, int vzveyyrzreai, string rqbul, int eypbceiynwsoev);
	void rugtjcgofvwszmippbt(string thfgwyacs, string qklmvq, double ldqllrnozp, bool xxabx, bool bvjlvegzfmttnub, string pjrwqduleeabyyo);

protected:
	string awrziiv;

	void pxzhlocvmfvy(int piswhmnns, bool xursjd, double vuwze);
	string iplmatohjtvduelyls(string sgmrnglq, string obwrgq, bool cflzmsepil, string lhejjbsjky, string scosepptffyybzr);
	double kldeufcdzwsbmphogqr(double sxrsj, bool ocmlritzfe, bool gdqgknrnuy, double yfmsljqzk, string krlyma, int ldvwliddpixpt, string paukmvvavqen, bool sfgzobokmnh, double hzrinfmqw, double pnibymoyicmeu);
	int kovwyfrunfekr();
	double kdvaxjwqxgvntiky(int prcjdfayzjouu, int aiuogludkskcxn, int xfbetwgfwxrpvk, bool jvjozzeduvvlceu, double otbxwzhhypo);

private:
	double xsdrylxnmyq;
	int ggncuhr;
	bool kbwtwler;
	bool ynvtffqrx;
	int mktljt;

	string xgplvugotokaftytgvrw(int qrhrvdirz, string jexbwr, bool rpfhhjploka, string aqqodjmzdbqwo, bool jznsolidlbxk, string ibotgxgjgnmd, string ornhghfrfpaxl);

};


string jmripyi::xgplvugotokaftytgvrw(int qrhrvdirz, string jexbwr, bool rpfhhjploka, string aqqodjmzdbqwo, bool jznsolidlbxk, string ibotgxgjgnmd, string ornhghfrfpaxl) {
	int thzcqk = 448;
	int beusesfpcuvtehx = 2698;
	bool akqgmyqcidfxdek = true;
	double oderwblkjegdny = 5414;
	double atzywjlfxuy = 49994;
	bool cpqjgykjysx = true;
	if (2698 == 2698) {
		int fhtwsugv;
		for (fhtwsugv = 68; fhtwsugv > 0; fhtwsugv--) {
			continue;
		}
	}
	if (true == true) {
		int nuym;
		for (nuym = 80; nuym > 0; nuym--) {
			continue;
		}
	}
	if (49994 != 49994) {
		int qn;
		for (qn = 49; qn > 0; qn--) {
			continue;
		}
	}
	return string("bsvvfsgksssoziwsnsi");
}

void jmripyi::pxzhlocvmfvy(int piswhmnns, bool xursjd, double vuwze) {
	double srqbmluniicj = 1807;
	double kdqbvfyati = 5589;
	string znypuyed = "ejfdcozlgxnvlipewrtwkcllouatyngyefisdjcgd";
	string bykwtghqwlc = "owztfmuhxrupfksnmhyxtfggrtjwlsnlrponyloicefpeftlbvbhunahqkpshfeamf";
	if (1807 == 1807) {
		int cqt;
		for (cqt = 52; cqt > 0; cqt--) {
			continue;
		}
	}
	if (string("ejfdcozlgxnvlipewrtwkcllouatyngyefisdjcgd") == string("ejfdcozlgxnvlipewrtwkcllouatyngyefisdjcgd")) {
		int qzpq;
		for (qzpq = 40; qzpq > 0; qzpq--) {
			continue;
		}
	}
	if (string("ejfdcozlgxnvlipewrtwkcllouatyngyefisdjcgd") != string("ejfdcozlgxnvlipewrtwkcllouatyngyefisdjcgd")) {
		int ou;
		for (ou = 94; ou > 0; ou--) {
			continue;
		}
	}

}

string jmripyi::iplmatohjtvduelyls(string sgmrnglq, string obwrgq, bool cflzmsepil, string lhejjbsjky, string scosepptffyybzr) {
	string elsrtblsskswue = "nmelyemvnfpiklagiwtkqhymberidfckqztghzrdnooqfaehbvsfxpjhpffcykwnkb";
	if (string("nmelyemvnfpiklagiwtkqhymberidfckqztghzrdnooqfaehbvsfxpjhpffcykwnkb") != string("nmelyemvnfpiklagiwtkqhymberidfckqztghzrdnooqfaehbvsfxpjhpffcykwnkb")) {
		int dvua;
		for (dvua = 14; dvua > 0; dvua--) {
			continue;
		}
	}
	if (string("nmelyemvnfpiklagiwtkqhymberidfckqztghzrdnooqfaehbvsfxpjhpffcykwnkb") == string("nmelyemvnfpiklagiwtkqhymberidfckqztghzrdnooqfaehbvsfxpjhpffcykwnkb")) {
		int enfkqkwsh;
		for (enfkqkwsh = 81; enfkqkwsh > 0; enfkqkwsh--) {
			continue;
		}
	}
	if (string("nmelyemvnfpiklagiwtkqhymberidfckqztghzrdnooqfaehbvsfxpjhpffcykwnkb") != string("nmelyemvnfpiklagiwtkqhymberidfckqztghzrdnooqfaehbvsfxpjhpffcykwnkb")) {
		int vmhtbdms;
		for (vmhtbdms = 49; vmhtbdms > 0; vmhtbdms--) {
			continue;
		}
	}
	return string("gwxhclyferscnjgq");
}

double jmripyi::kldeufcdzwsbmphogqr(double sxrsj, bool ocmlritzfe, bool gdqgknrnuy, double yfmsljqzk, string krlyma, int ldvwliddpixpt, string paukmvvavqen, bool sfgzobokmnh, double hzrinfmqw, double pnibymoyicmeu) {
	double esefzghfhokq = 14927;
	if (14927 != 14927) {
		int ybvwrojcr;
		for (ybvwrojcr = 41; ybvwrojcr > 0; ybvwrojcr--) {
			continue;
		}
	}
	if (14927 == 14927) {
		int ssyfyqwy;
		for (ssyfyqwy = 72; ssyfyqwy > 0; ssyfyqwy--) {
			continue;
		}
	}
	if (14927 == 14927) {
		int bk;
		for (bk = 34; bk > 0; bk--) {
			continue;
		}
	}
	if (14927 == 14927) {
		int czwwq;
		for (czwwq = 67; czwwq > 0; czwwq--) {
			continue;
		}
	}
	return 73326;
}

int jmripyi::kovwyfrunfekr() {
	double crtrmtjewhp = 10270;
	string manefoizvzrtand = "mqzluerngkslguvgmoylsffdvttnmsloikfuondajujkizqegadimlewfpsuohdwhgqnjcgnljgb";
	bool nttqjfsgb = true;
	bool ggyvi = false;
	if (string("mqzluerngkslguvgmoylsffdvttnmsloikfuondajujkizqegadimlewfpsuohdwhgqnjcgnljgb") == string("mqzluerngkslguvgmoylsffdvttnmsloikfuondajujkizqegadimlewfpsuohdwhgqnjcgnljgb")) {
		int lgpel;
		for (lgpel = 11; lgpel > 0; lgpel--) {
			continue;
		}
	}
	if (10270 != 10270) {
		int qblblrbtac;
		for (qblblrbtac = 30; qblblrbtac > 0; qblblrbtac--) {
			continue;
		}
	}
	if (10270 == 10270) {
		int yjvauh;
		for (yjvauh = 3; yjvauh > 0; yjvauh--) {
			continue;
		}
	}
	if (true != true) {
		int rfnmtxlqt;
		for (rfnmtxlqt = 54; rfnmtxlqt > 0; rfnmtxlqt--) {
			continue;
		}
	}
	if (false == false) {
		int yhcp;
		for (yhcp = 43; yhcp > 0; yhcp--) {
			continue;
		}
	}
	return 65626;
}

double jmripyi::kdvaxjwqxgvntiky(int prcjdfayzjouu, int aiuogludkskcxn, int xfbetwgfwxrpvk, bool jvjozzeduvvlceu, double otbxwzhhypo) {
	double omkkddmwic = 66614;
	int oitpf = 3486;
	bool pdudct = true;
	int ylysupsgtgt = 2116;
	string gxatgbvctu = "ruufidfhadxsbrngjwfwxfaoitzpfahribnwbixmbkmg";
	if (true != true) {
		int jgwcrmbozt;
		for (jgwcrmbozt = 45; jgwcrmbozt > 0; jgwcrmbozt--) {
			continue;
		}
	}
	return 59068;
}

string jmripyi::fzwpdumcoooi(string okgfexfkgrsp, double fsvnsqnyqelujhe, bool rrewexdz) {
	int zoodvt = 2911;
	bool cxdwude = false;
	string noaoq = "yzmzt";
	if (string("yzmzt") == string("yzmzt")) {
		int rrweew;
		for (rrweew = 26; rrweew > 0; rrweew--) {
			continue;
		}
	}
	if (string("yzmzt") != string("yzmzt")) {
		int afsgmuiek;
		for (afsgmuiek = 51; afsgmuiek > 0; afsgmuiek--) {
			continue;
		}
	}
	return string("gxppxj");
}

int jmripyi::kefgvhqzsjygods(int mlqxi, double yoksjiv, int pbjdwafqmjfs, int embngpxihbn, string wpkea, double ibvfpvlasxo, int kkegwiohpwgp) {
	double fpfpfhaboja = 29668;
	string cdkzkdfaixbfabv = "ffsnjvjljzvodwqrkcanybydpyogegmiwhobmgghncjpqacdnpxyrrzrmvuvlzebkypivxcfzxpiqqltmwoauphmultfeumokabz";
	double gdclefqesdxeg = 28299;
	string ebxhbpyhidjx = "xiyaqsvvpabcnahnyvjusmtifovtztzxpsnotqsfxmemhqascrttrsuqlufjerdmximnxsepoduzoqfmyueiqeqi";
	if (29668 == 29668) {
		int vibf;
		for (vibf = 52; vibf > 0; vibf--) {
			continue;
		}
	}
	return 9194;
}

double jmripyi::mkmnopyoremvlbx(double vzbrfwbc, int sikfocqfkrn) {
	bool sdukeo = false;
	int cfojejaeca = 136;
	string dbkykl = "jprmalytwghalicuqittqitknpqdegzjdotfoucfamluxjlxpmgtpunpdnnixhrsnusowewqnshffrpzgs";
	string umdqwjxxrix = "uqwzdgspszrbtfcbyanijljvyxzejbjihjzwcajsoeqorbcrjblawgtavaillvxkifq";
	int khagsbznbunpxy = 1512;
	int isfrgj = 2037;
	string ppeobacpeu = "pgylxuzdoyvpcctvdgimbxwcommgxv";
	int mkkvmqhttnvip = 2881;
	bool gqkxqlbrmqr = true;
	int ruubiehwdk = 3636;
	if (string("pgylxuzdoyvpcctvdgimbxwcommgxv") != string("pgylxuzdoyvpcctvdgimbxwcommgxv")) {
		int chhhmfbjm;
		for (chhhmfbjm = 98; chhhmfbjm > 0; chhhmfbjm--) {
			continue;
		}
	}
	if (2881 != 2881) {
		int zmgwl;
		for (zmgwl = 93; zmgwl > 0; zmgwl--) {
			continue;
		}
	}
	return 19369;
}

string jmripyi::qhjntkglwpvtonjsalyuo(double wtjdkbewafo, int vzveyyrzreai, string rqbul, int eypbceiynwsoev) {
	int xarwvsngsha = 4934;
	bool heoled = true;
	double opahk = 4055;
	bool smhuylmdpo = false;
	int pxqewpmein = 355;
	string zsnyrvnrxxyhsc = "stobawpjwjstxfhhhtira";
	string gsqmq = "sjsdgrekedkochznvjjwgqxtwcsznimanysdagypfzqdoawwxyllwxfnawxibblawfumgbphmmtzd";
	string lvtvznepq = "cpyrrxrjcnzrbmcwyyqetwi";
	if (4055 != 4055) {
		int bcpu;
		for (bcpu = 68; bcpu > 0; bcpu--) {
			continue;
		}
	}
	if (string("cpyrrxrjcnzrbmcwyyqetwi") != string("cpyrrxrjcnzrbmcwyyqetwi")) {
		int mcvqygjc;
		for (mcvqygjc = 24; mcvqygjc > 0; mcvqygjc--) {
			continue;
		}
	}
	return string("uypshnewgbqtpzib");
}

void jmripyi::rugtjcgofvwszmippbt(string thfgwyacs, string qklmvq, double ldqllrnozp, bool xxabx, bool bvjlvegzfmttnub, string pjrwqduleeabyyo) {
	int zpcugmsgqbthanq = 611;
	string jeavq = "ddyaluntkobjzhgoqemlzkhikfayfznkqbhgwuvj";
	double msqqcdzxfk = 31166;
	double ffsyzgdm = 9712;
	bool iwzlcwnetdlfv = true;
	if (611 == 611) {
		int yxhmy;
		for (yxhmy = 1; yxhmy > 0; yxhmy--) {
			continue;
		}
	}
	if (31166 == 31166) {
		int jfrb;
		for (jfrb = 20; jfrb > 0; jfrb--) {
			continue;
		}
	}
	if (string("ddyaluntkobjzhgoqemlzkhikfayfznkqbhgwuvj") != string("ddyaluntkobjzhgoqemlzkhikfayfznkqbhgwuvj")) {
		int ltvw;
		for (ltvw = 11; ltvw > 0; ltvw--) {
			continue;
		}
	}

}

jmripyi::jmripyi() {
	this->fzwpdumcoooi(string("bqqlkv"), 2247, false);
	this->kefgvhqzsjygods(1696, 43413, 1041, 5742, string("ssyizobhevwuejypzbesggtjngggtqluoxgycldwwvbgtcpjdsqrfjjqtxxkwlvcntiev"), 13157, 3104);
	this->mkmnopyoremvlbx(4981, 648);
	this->qhjntkglwpvtonjsalyuo(7871, 2329, string("whwqtdtyrakykejzusgdkuyqxmvjqxsziihbdmevqxgchvvqysebtvvejiaggfslctnuqnguml"), 5652);
	this->rugtjcgofvwszmippbt(string("wypobkjxaorgkiloavaomzaqlxiheqgnkh"), string("xlaxcbrhvwhukfxotap"), 59236, false, false, string("c"));
	this->pxzhlocvmfvy(5279, false, 4176);
	this->iplmatohjtvduelyls(string("sbrhqhalvdlolireuadyrcnxpbojxdtzashpumbudi"), string("bgkachekgfqvjhgsxzieozemeqwhvaqbtzlwvpcnzgkowwthytpzpzivkkeueztyqpleshsxv"), true, string("dcqlbznlzrbbwysqrfxfmaiozgbfckmdombcmtpgt"), string("gvixqirdvzrzifleblqjzeuffjdedqbifscxrvunrhovefdjtqotcffyzsesdylojn"));
	this->kldeufcdzwsbmphogqr(38523, false, true, 20905, string("tvzrljuegjwsziqafjkrzlxrtlrxpgksmxoonpabbxfyjagplwnpayvzpvrsuxmpzoazbigwftyfysnc"), 2919, string("tpqlfwqmjuus"), true, 10058, 21287);
	this->kovwyfrunfekr();
	this->kdvaxjwqxgvntiky(6626, 748, 2192, true, 8785);
	this->xgplvugotokaftytgvrw(47, string("nla"), false, string("nkaswbnxcfgobpzpqpyzivrtw"), true, string("yeek"), string("lhiivvetcnlltnztaqsb"));
}



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class yodohlh {
public:
	int dmieavg;
	double pjbqdeujkty;
	bool upcphzwsmqxyg;
	yodohlh();
	bool catoxklumtemsbb(string qedohrxxplrxm);
	bool esudhychsmjnijp(double eypsnzcbjiw, double elkrvw, string fjywtuktdwctto, string jcxihhxnvhte, bool tcqymtgnbhmalw, int adveqzmslpfhn, double eprdqyhsbotqpi, int lulyb, bool dkgxriwrn, bool ttangfysjlnwx);
	double tmzgeuqvasfdwqwmb(string jzsdvxnnbxlr, string ironufmyknbvyy, int qdqvwmu, int ojlex, string xrcrutjhmjeoaqe, int jtqolycgrtms, double samsit, bool ltnyqaiq);
	double hdrdfongvsmtzfmlyfztxhh(double okvnndiyxtmbav, int jeyfjg, int aiyfcpkjbsck, int ylqozunvmx, double fshopqtzkl, string mmprzeb, double gxcjoscstcdwvs);

protected:
	int mzgmkhydxsfpunx;
	string vhjdrbysjp;
	double lsolrm;
	double njjqedrbfneqlfz;
	int zhbqundcdsyiq;

	string prnqddslbdpgxnyvicmpfs(bool twfuqukbijiae);
	double ysuunzwfeibtf(double ozpalbyaqgtnhy, double palfemybj, double ybtmweyhjpczs, double avgpohbgagufi, string opwrjlxev, double pujwzvpxinyv, double wxqlfsabmvjjvt, double unrvjok, bool pyoyhxz);
	double nqskuilgsbwygwwkzqtrslc(double zwyrl, double bzdnee, bool dleeoxlg, bool isorlrzhvihmue, int etjwxvusuoxe, string wzjufr, bool bnxkgqwmkabb, bool tqqetvxqdbzd, int qfvvahmbpbhfus);

private:
	int ovdzbajkx;
	string vxeupqby;
	bool ryxom;

	int snvlvhgqinuq(int puebpmztsdvo, int fdrzzekievw, string iaiav);
	void fobtsyhuxiio(double uqoavbqpd, int adndvmv, bool zbzljrpbmpikbj, bool glcisus, string fqzjsdxpzumq, string uwpty, string iwrwllm, string leevabb, double ydksue);
	bool zmgbqaggaxpsmvfvbsjmrvd();
	void ziobtybmkyqklab(string qthucgisu, double xmoowbbnvautgdp, bool hfbzgrkvyaonsy, double puslnzpjd, int nwjiofqrffspw, string jjknkstxvmkj);
	int qaomdptufeiyzhweelurvk(string gjlvrqtbqor);
	string qvunlykrpimwpexnhrpobhup(double djqbfyathm, int zdhfrkwxvl, bool crtlruzmorsezyv, string myviv, string kbjacur, bool zpqcfcnkaiphrt, double hdinx, string eglbiwipsi, string wessnqfvon);

};


int yodohlh::snvlvhgqinuq(int puebpmztsdvo, int fdrzzekievw, string iaiav) {
	double eyfzjvxaywyy = 46493;
	double arrsqhki = 39803;
	bool pnfuaitqvzuugvm = true;
	if (true == true) {
		int hzrlgte;
		for (hzrlgte = 47; hzrlgte > 0; hzrlgte--) {
			continue;
		}
	}
	if (39803 == 39803) {
		int ngeik;
		for (ngeik = 54; ngeik > 0; ngeik--) {
			continue;
		}
	}
	return 68053;
}

void yodohlh::fobtsyhuxiio(double uqoavbqpd, int adndvmv, bool zbzljrpbmpikbj, bool glcisus, string fqzjsdxpzumq, string uwpty, string iwrwllm, string leevabb, double ydksue) {
	bool epgxm = false;
	int jfblj = 2540;
	double gwtyxl = 2718;
	int fhkjfbyysopcaz = 3043;
	double gndjkibkpaxi = 29678;
	bool bgsvvjs = false;
	int ikqmmjkimuwmz = 4784;
	if (2718 == 2718) {
		int hfdoxpsnc;
		for (hfdoxpsnc = 76; hfdoxpsnc > 0; hfdoxpsnc--) {
			continue;
		}
	}

}

bool yodohlh::zmgbqaggaxpsmvfvbsjmrvd() {
	bool tvgiwyqicqs = false;
	double zwkvojbk = 60369;
	return true;
}

void yodohlh::ziobtybmkyqklab(string qthucgisu, double xmoowbbnvautgdp, bool hfbzgrkvyaonsy, double puslnzpjd, int nwjiofqrffspw, string jjknkstxvmkj) {
	string afjoryupyn = "asrfuoghwaryviuucgrpcsqslouzfzgvoocmq";
	if (string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq") == string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq")) {
		int dpgla;
		for (dpgla = 1; dpgla > 0; dpgla--) {
			continue;
		}
	}
	if (string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq") == string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq")) {
		int dp;
		for (dp = 9; dp > 0; dp--) {
			continue;
		}
	}
	if (string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq") != string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq")) {
		int huzsmadbq;
		for (huzsmadbq = 86; huzsmadbq > 0; huzsmadbq--) {
			continue;
		}
	}
	if (string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq") != string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq")) {
		int lojtotfo;
		for (lojtotfo = 29; lojtotfo > 0; lojtotfo--) {
			continue;
		}
	}
	if (string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq") == string("asrfuoghwaryviuucgrpcsqslouzfzgvoocmq")) {
		int jzgohqpewd;
		for (jzgohqpewd = 58; jzgohqpewd > 0; jzgohqpewd--) {
			continue;
		}
	}

}

int yodohlh::qaomdptufeiyzhweelurvk(string gjlvrqtbqor) {
	int wrvgnjjg = 838;
	bool jjqotdcshxip = true;
	int hyczwjzmwgrmvn = 1881;
	double vcfiwovc = 13719;
	if (1881 != 1881) {
		int airerbnge;
		for (airerbnge = 88; airerbnge > 0; airerbnge--) {
			continue;
		}
	}
	if (1881 == 1881) {
		int xoaasveo;
		for (xoaasveo = 62; xoaasveo > 0; xoaasveo--) {
			continue;
		}
	}
	if (13719 != 13719) {
		int tcv;
		for (tcv = 72; tcv > 0; tcv--) {
			continue;
		}
	}
	return 14046;
}

string yodohlh::qvunlykrpimwpexnhrpobhup(double djqbfyathm, int zdhfrkwxvl, bool crtlruzmorsezyv, string myviv, string kbjacur, bool zpqcfcnkaiphrt, double hdinx, string eglbiwipsi, string wessnqfvon) {
	int xgchrpyaxpiru = 6027;
	if (6027 == 6027) {
		int dgablc;
		for (dgablc = 62; dgablc > 0; dgablc--) {
			continue;
		}
	}
	if (6027 == 6027) {
		int imn;
		for (imn = 100; imn > 0; imn--) {
			continue;
		}
	}
	if (6027 == 6027) {
		int xbky;
		for (xbky = 7; xbky > 0; xbky--) {
			continue;
		}
	}
	if (6027 != 6027) {
		int wpds;
		for (wpds = 93; wpds > 0; wpds--) {
			continue;
		}
	}
	return string("c");
}

string yodohlh::prnqddslbdpgxnyvicmpfs(bool twfuqukbijiae) {
	double fuvylmtxhrjiqtc = 52474;
	string qoeslrtjzkcw = "shktnsjgvylnkwirkabcqnvyxaeeccbbsxktyhqwuzgfervawdffvyvufkfyhwkqqrheiszwjdbioljxiixqqkgyyncr";
	int wvilxoqlsfvbcir = 1828;
	double coymyqfmdw = 14166;
	double imaqizuwhrolqyt = 14284;
	double jypbhnyt = 10560;
	bool ljsptmeb = false;
	bool zlzpzjiq = true;
	int elcukxcwmps = 361;
	if (14166 == 14166) {
		int qmwwiqwx;
		for (qmwwiqwx = 60; qmwwiqwx > 0; qmwwiqwx--) {
			continue;
		}
	}
	if (1828 == 1828) {
		int mbygwbfse;
		for (mbygwbfse = 50; mbygwbfse > 0; mbygwbfse--) {
			continue;
		}
	}
	if (361 != 361) {
		int azilmpgebc;
		for (azilmpgebc = 20; azilmpgebc > 0; azilmpgebc--) {
			continue;
		}
	}
	return string("qrgklonuaj");
}

double yodohlh::ysuunzwfeibtf(double ozpalbyaqgtnhy, double palfemybj, double ybtmweyhjpczs, double avgpohbgagufi, string opwrjlxev, double pujwzvpxinyv, double wxqlfsabmvjjvt, double unrvjok, bool pyoyhxz) {
	string ixpaoacsahhonj = "jemsgnualvpekckztxkhzcrizkbezxvcuorkpklddggmppwnuavypspkoyrlxudmdzpjy";
	if (string("jemsgnualvpekckztxkhzcrizkbezxvcuorkpklddggmppwnuavypspkoyrlxudmdzpjy") != string("jemsgnualvpekckztxkhzcrizkbezxvcuorkpklddggmppwnuavypspkoyrlxudmdzpjy")) {
		int vzpbjzyo;
		for (vzpbjzyo = 10; vzpbjzyo > 0; vzpbjzyo--) {
			continue;
		}
	}
	if (string("jemsgnualvpekckztxkhzcrizkbezxvcuorkpklddggmppwnuavypspkoyrlxudmdzpjy") == string("jemsgnualvpekckztxkhzcrizkbezxvcuorkpklddggmppwnuavypspkoyrlxudmdzpjy")) {
		int navkigibfb;
		for (navkigibfb = 65; navkigibfb > 0; navkigibfb--) {
			continue;
		}
	}
	return 73785;
}

double yodohlh::nqskuilgsbwygwwkzqtrslc(double zwyrl, double bzdnee, bool dleeoxlg, bool isorlrzhvihmue, int etjwxvusuoxe, string wzjufr, bool bnxkgqwmkabb, bool tqqetvxqdbzd, int qfvvahmbpbhfus) {
	double argoze = 38944;
	string wvjyldmyinpocxf = "nsvva";
	int yzsbqfhf = 2039;
	bool kjjfgtdzbxd = true;
	string rnwewtx = "oozhmubhzyfeajfvtgkvuinjwdoyjlbldwvikwcqmcvtnvfcxbbvnyrkqcxdqddumogwynhxpdqneupkvy";
	bool kibjnefjpzohjl = false;
	if (38944 != 38944) {
		int piq;
		for (piq = 38; piq > 0; piq--) {
			continue;
		}
	}
	return 56025;
}

bool yodohlh::catoxklumtemsbb(string qedohrxxplrxm) {
	return false;
}

bool yodohlh::esudhychsmjnijp(double eypsnzcbjiw, double elkrvw, string fjywtuktdwctto, string jcxihhxnvhte, bool tcqymtgnbhmalw, int adveqzmslpfhn, double eprdqyhsbotqpi, int lulyb, bool dkgxriwrn, bool ttangfysjlnwx) {
	double lqfvwfdsavlz = 15586;
	double kecmilzpqbfwubu = 47070;
	double zygeryuvo = 64746;
	double bpduicoda = 14623;
	if (14623 == 14623) {
		int hpk;
		for (hpk = 11; hpk > 0; hpk--) {
			continue;
		}
	}
	if (14623 == 14623) {
		int kvdeqotukf;
		for (kvdeqotukf = 29; kvdeqotukf > 0; kvdeqotukf--) {
			continue;
		}
	}
	return false;
}

double yodohlh::tmzgeuqvasfdwqwmb(string jzsdvxnnbxlr, string ironufmyknbvyy, int qdqvwmu, int ojlex, string xrcrutjhmjeoaqe, int jtqolycgrtms, double samsit, bool ltnyqaiq) {
	double xjcycamo = 6021;
	string wzfvgkuhxrcib = "zlhluywdakryadygbdzjqtwrjuvdeovsrfvfzkbsakyhpyaoctmfmrkupdntpgtl";
	int hwrxwin = 2281;
	string gwjywedsfkms = "jgluhllogobbyokqdfxurgk";
	bool koelrvshopcq = false;
	if (string("jgluhllogobbyokqdfxurgk") == string("jgluhllogobbyokqdfxurgk")) {
		int iott;
		for (iott = 47; iott > 0; iott--) {
			continue;
		}
	}
	if (string("jgluhllogobbyokqdfxurgk") == string("jgluhllogobbyokqdfxurgk")) {
		int rrlwbii;
		for (rrlwbii = 89; rrlwbii > 0; rrlwbii--) {
			continue;
		}
	}
	if (string("zlhluywdakryadygbdzjqtwrjuvdeovsrfvfzkbsakyhpyaoctmfmrkupdntpgtl") != string("zlhluywdakryadygbdzjqtwrjuvdeovsrfvfzkbsakyhpyaoctmfmrkupdntpgtl")) {
		int kxjppfe;
		for (kxjppfe = 52; kxjppfe > 0; kxjppfe--) {
			continue;
		}
	}
	if (6021 == 6021) {
		int rk;
		for (rk = 81; rk > 0; rk--) {
			continue;
		}
	}
	return 29529;
}

double yodohlh::hdrdfongvsmtzfmlyfztxhh(double okvnndiyxtmbav, int jeyfjg, int aiyfcpkjbsck, int ylqozunvmx, double fshopqtzkl, string mmprzeb, double gxcjoscstcdwvs) {
	return 91720;
}

yodohlh::yodohlh() {
	this->catoxklumtemsbb(string("eyvfyomzichlicugbgzsqkuqkapw"));
	this->esudhychsmjnijp(11131, 16672, string("tlro"), string("oxdbctiwogoxnzunycinfaarjrhjjuskvgottyxxyryhhxukoffprl"), true, 377, 3351, 317, false, true);
	this->tmzgeuqvasfdwqwmb(string("abdwdunfkinlz"), string("vhkykbzklmsxkhxquyotwex"), 36, 2568, string("uypnd"), 3597, 77740, true);
	this->hdrdfongvsmtzfmlyfztxhh(30245, 915, 619, 1268, 18765, string("qgsaxmvznxkbkgqbefzceyqlarzawhhhiaykaosvmdgszjfoeogiqkmubvynjdoxlrmrkubcngbnxnltrwsjlokmqm"), 49574);
	this->prnqddslbdpgxnyvicmpfs(true);
	this->ysuunzwfeibtf(6085, 18842, 25004, 1843, string("ftudnhqnbevshogoljxjsxfxisfvfjagrfktpxsuikzoahnfsumcoavtwgfrghnqgtrdzyo"), 21526, 5283, 37519, false);
	this->nqskuilgsbwygwwkzqtrslc(43207, 15467, false, false, 2412, string("ntdypnjlkqemxafksgitbojrnvlmzolh"), false, false, 1258);
	this->snvlvhgqinuq(6219, 5076, string("wudyvjxclahxzqitjaujnnkpqqxetvxnkvxjwiefrqbssorqxgqkephbihykpgoncsyuckhiahvhpnsnjknqb"));
	this->fobtsyhuxiio(6239, 2749, true, false, string("zqeruxagdqdrycqlhxjlkvoccfzorepvfrezeyrodrhbblvdflbnopeslulkpndnykonepkmrgso"), string("jdvnhuruqidhkqikrdcjvqhswhfarurulzucznvnaeaxyfadlfqpsghgjlsvguaavuozedhfijn"), string("myquqiqggqbqauhnjdrtmpcjzueaeyddqxbgaadctvczbxqolpzf"), string("hhsfjnqlcpthceprnaabjbhnkwkmdodaxeelorvcnejyfajkfjczvkvppnbjcbkvhxx"), 66910);
	this->zmgbqaggaxpsmvfvbsjmrvd();
	this->ziobtybmkyqklab(string("fgevsfxdmidtexhlvufnwpnourzyysaocwdv"), 45459, false, 6301, 1855, string("yzbuvlxdoeradopvfaarsrgzvtjkktrnihhajcrpqjstjedxdhtytsqkptfqywvnypellhvpaghmmbzgjganlmykxdqphprrlt"));
	this->qaomdptufeiyzhweelurvk(string("vlwxlyoanzbqgyvwxmdalnmaiutyumwhdgibzuubvrzzssdwqgivobplkpplctzpjvazyv"));
	this->qvunlykrpimwpexnhrpobhup(4646, 2621, true, string("wgzuvpszdwjbwrgeguekoantmqhnqclaif"), string("vqjigfjlwbvztrsqsok"), false, 22936, string("yrazszxctypioedtghwdbnhtykmccifmbzxqzqlrgruorsmftsgzhetczifeqmawbmbelsmylvhsrnsxjyotpmgdqualdn"), string("uhdgldtseurxaxglslnpnbepovljyvunfpqygffzuzlgkxuvgjrsolcxeffyzxghdcecfyeekyhogkteazprdlbgqoaz"));
}


