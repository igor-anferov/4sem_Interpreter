/*
 
 P	→	program I ; D1 ; B⟂
 D1	→	var D { , D }
 D	→	X1 { , X1 } : [ int | bool | real ]
 X1	→	I | I '[' N ']'
 X2	→	I | I '[' E ']'
 B	→	begin [ [ S ] {  ; [ S ] } ] end
 S	→	X2:=E | if E then S [ else S ] | while E do S | B | read (X2) | write (E)
 E	→	E1 [ = | < | > | != ] E1 | E1
 E1	→	T { [ + | - | or ] T }
 T	→	F1 { [ * | / | % | and ] F1 }
 F1	→	[+|-] F
 F	→	X2 | N | L | not F | (E)
 L	→	true | false
 I	→	C | IC | IR
 N	→	R | NR
 C	→	a | b | ... | z | A | B | ... | Z
 R	→	0 | 1 | 2 | ... | 9
 
 */


#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

template <class T, int max_size >
class Stack {
    T s[max_size];
    int top;
    
public:
    Stack() {
        top = 0;
    }
    
    void reset() {
        top = 0;
    }
    
    void push(T i) {
        if ( !is_full() ) {
            s[top] = i;
            ++top;
        }
        else
            throw "Stack_is_full";
    }
    
    T pop() {
        if ( !is_empty() ) {
            --top;
            return s[top];
        }
        else
            throw "Stack_is_empty";
    }
    
    bool is_empty() {
        return top == 0;
    }
    
    bool is_full() {
        return top == max_size;
    }
};

class lex_val {
    union {
        int i;
        double r;
    };
    bool is_re;
public:
    lex_val(int x=0) {
        i=x;
        is_re=0;
    };
    lex_val(double x) {
        r=x;
        is_re=1;
    };
    operator int() {
        return i;
    }
    operator bool() {
        return i;
    }
    operator double() {
        return r;
    }
    void operator= (int x) {
        is_re?r=x:i=x;
    }
    void operator= (double x) {
        r=x;
        is_re=1;
    }
    lex_val operator+ (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r+(double)sec;
            else
                return r+(int)sec;
            else
                if (sec.is_real())
                    return i+(double)sec;
                else
                    return i+(int)sec;
    }
    lex_val operator* (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r*(double)sec;
            else
                return r*(int)sec;
            else
                if (sec.is_real())
                    return i*(double)sec;
                else
                    return i*(int)sec;
    }
    lex_val operator- (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r-(double)sec;
            else
                return r-(int)sec;
            else
                if (sec.is_real())
                    return i-(double)sec;
                else
                    return i-(int)sec;
    }
    lex_val operator/ (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r/(double)sec;
            else
                return r/(int)sec;
            else
                if (sec.is_real())
                    return i/(double)sec;
                else
                    return i/(int)sec;
    }
    lex_val operator% (lex_val sec) {
        return i%(int)sec;
    }
    bool operator== (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r==(double)sec;
            else
                return r==(int)sec;
            else
                if (sec.is_real())
                    return i==(double)sec;
                else
                    return i==(int)sec;
    }
    bool operator<= (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r<=(double)sec;
            else
                return r<=(int)sec;
            else
                if (sec.is_real())
                    return i<=(double)sec;
                else
                    return i<=(int)sec;
    }
    bool operator>= (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r>=(double)sec;
            else
                return r>=(int)sec;
            else
                if (sec.is_real())
                    return i>=(double)sec;
                else
                    return i>=(int)sec;
    }
    bool operator< (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r<(double)sec;
            else
                return r<(int)sec;
            else
                if (sec.is_real())
                    return i<(double)sec;
                else
                    return i<(int)sec;
    }
    bool operator> (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r>(double)sec;
            else
                return r>(int)sec;
            else
                if (sec.is_real())
                    return i>(double)sec;
                else
                    return i>(int)sec;
    }
    bool operator!= (lex_val sec) {
        if (is_re)
            if (sec.is_real())
                return r!=(double)sec;
            else
                return r!=(int)sec;
            else
                if (sec.is_real())
                    return i!=(double)sec;
                else
                    return i!=(int)sec;
    }
    
    
    void decReal() {
        is_re=1;
    }
    bool is_real() {
        return is_re;
    }
};

enum type_of_lex {
    LEX_NULL,       // 0
    
    LEX_PROGRAM,    // 1
    LEX_VAR,        // 2
    LEX_BEGIN,      // 3
    LEX_END,        // 4
    LEX_FIN,        // 5
    
    LEX_BOOL,       // 6
    LEX_INT,        // 7
    LEX_REAL,       // 8
    
    LEX_WHILE,      // 9
    LEX_DO,         // 10
    LEX_IF,         // 11
    LEX_THEN,       // 12
    LEX_ELSE,       // 13
    
    LEX_TRUE,       // 14
    LEX_FALSE,      // 15
    
    LEX_READ,       // 16
    LEX_WRITE,      // 17
    
    LEX_NOT,        // 18
    LEX_AND,        // 19
    LEX_OR,         // 20
    
    LEX_SEMICOLON,  // 21   ;
    LEX_COMMA,      // 22   ,
    LEX_COLON,      // 23   :
    LEX_ASSIGN,     // 24   :=
    LEX_LPAREN,     // 25   (
    LEX_RPAREN,     // 26   )
    LEX_SQ_LPAREN,  // 27   (
    LEX_SQ_RPAREN,  // 28   )
    LEX_PLUS,       // 29   +
    LEX_UN_PLUS,    // 30   +
    LEX_MINUS,      // 31   -
    LEX_UN_MINUS,   // 32   -
    LEX_TIMES,      // 33   *
    LEX_SLASH,      // 34   /
    LEX_PERCENT,    // 35   %
    
    LEX_EQ,         // 36   ==
    LEX_NEQ,        // 37   !=
    LEX_LSS,        // 38   <
    LEX_GTR,        // 39   >
    LEX_LEQ,        // 40   <=
    LEX_GEQ,        // 41   >=
    
    LEX_INT_NUM,    // 42
    LEX_REAL_NUM,   // 43
    LEX_ID,         // 44
    LEX_INDEX,      // 45
    
    POLIZ_LABEL,    // 46
    POLIZ_ADDRESS,  // 47
    POLIZ_GO,       // 48
    POLIZ_FGO       // 49
};

class Scanner;

class Lex {
    type_of_lex t_lex;
    lex_val v_lex;
    
public:
    Lex ( type_of_lex t = LEX_NULL, int v=0): v_lex(v) {
        t_lex = t;
    }
    Lex ( type_of_lex t, double v): v_lex(v) {
        t_lex = t;
    }
    type_of_lex get_type () {
        return t_lex;
    }
    lex_val get_value () {
        return v_lex;
    }
    friend ostream& operator << ( ostream &s, Lex l );
};

class Ident {
    char * name;
    bool declare;
    type_of_lex type;
    bool * assign;
    lex_val * value;
    int dim;
    
public:
    Ident () {
        declare = false;
        assign = NULL;
        value = NULL;
        dim=1;
    }
    ~Ident() {
        delete [] assign;
        delete [] value;
    }
    char *get_name () {
        return name;
    }
    void put_name (const char *n) {
        name = new char [ strlen(n) + 1 ];
        strcpy ( name, n );
    }
    void put_dim (int n) {
        dim=n;
    }
    int get_dim () {
        return dim;
    }
    bool get_declare () {
        return declare;
    }
    void put_declare () {
        declare = true;
    }
    type_of_lex get_type () {
        return type;
    }
    void put_type ( type_of_lex t ) {
        type = t;
        assign = new bool[dim];
        value = new lex_val[dim];
        for (int i=0; i<dim; i++)
            assign[i]=false;
        if (t==LEX_REAL)
            for (int i=0; i<dim; i++)
                value[i].decReal();
    }
    bool get_assign (int n) {
        if (n>=dim)
            throw "ERROR! Out of range!";
        return assign[n];
    }
    void put_assign (int n) {
        if (n>=dim)
            throw "ERROR! Out of range!";
        assign[n] = true;
    }
    lex_val get_value (int n) {
        if (n>=dim)
            throw "ERROR! Out of range!";
        return value[n];
    }
    void put_value (int n, int v) {
        if (n>=dim)
            throw "ERROR! Out of range!";
        value[n] = v;
    }
    void put_value (int n, double v) {
        if (n>=dim)
            throw "ERROR! Out of range!";
        value[n] = v;
    }
};

class tabl_Ident {
    Ident * p;
    int size;
    int top;
    
public:
    tabl_Ident ( int max_size ) {
        p = new Ident[size=max_size];
        top = 1;
    }
    ~tabl_Ident () {
        delete [] p;
    }
    Ident& operator[] ( int k ) {
        return p[k];
    }
    int put ( const char *buf) {
        if (top==size)
            throw "tabl_Ident overflow!!!";
        for ( int j=1; j<top; ++j )
            if ( !strcmp(buf, p[j].get_name()) )
                return j;
        p[top].put_name(buf);
        ++top;
        return top-1;
    }
};

class Scanner {
    enum state { H, IDENT, COM, ALE, DELIM, NEQ };
    state CS;
    FILE * fp;
    int c;
    char buf[80];
    int buf_top;
    
    void clear () {
        buf_top = 0;
        memset(buf, 0, sizeof(buf));
    }
    void add () {
        buf [ buf_top ++ ] = c;
    }
    int look ( const char *buf, const char **list ) {
        int i = 0;
        while ( list[i] ) {
            if ( !strcmp(buf, list[i]) )
                return i;
            ++i;
        }
        return 0;
    }
    void gc () {
        c = fgetc (fp);
    }
    
public:
    static const char * TW[];
    static const char * TP[];
    static type_of_lex words[];
    static const char * TD[];
    static type_of_lex dlms[];
    
    Lex get_lex ();
    Scanner ( const char * program ) {
        fp = fopen ( program, "r" );
        CS = H;
        clear();
        gc();
    }
};

const char * Scanner::TW[] = {
    "",         // 0
    "and",      // 1
    "begin",    // 2
    "bool",     // 3
    "do",       // 4
    "else",     // 5
    "end",      // 6
    "if",       // 7
    "false",    // 8
    "int",      // 9
    "not",      // 10
    "or",       // 11
    "program",  // 12
    "read",     // 13
    "then",     // 14
    "true",     // 15
    "var",      // 16
    "while",    // 17
    "write",    // 18
    "real",     // 19
    NULL
};

const char * Scanner:: TD[] = {
    "",     // 0
    ";",    // 1
    ",",    // 2
    ":",    // 3
    ":=",   // 4
    "(",    // 5
    ")",    // 6
    "=",    // 7
    "<",    // 8
    ">",    // 9
    "+",    // 10
    "-",    // 11
    "+@",   // 12
    "-@",   // 13
    "*",    // 14
    "/",    // 15
    "%",    // 16
    "<=",   // 17
    "!=",   // 18
    ">=",   // 19
    "[",    // 20
    "]",    // 21
    NULL
};

const char * Scanner::TP[] = {
    "LEX_ID",       // 0
    "[]",           // 1
    "LABEL",        // 2
    "ADDRESS",      // 3
    "!",            // 4
    "!F",           // 5
    NULL
};

tabl_Ident TID(1000);

type_of_lex Scanner::words[] = {
    LEX_NULL,
    LEX_AND,
    LEX_BEGIN,
    LEX_BOOL,
    LEX_DO,
    LEX_ELSE,
    LEX_END,
    LEX_IF,
    LEX_FALSE,
    LEX_INT,
    LEX_NOT,
    LEX_OR,
    LEX_PROGRAM,
    LEX_READ,
    LEX_THEN,
    LEX_TRUE,
    LEX_VAR,
    LEX_WHILE,
    LEX_WRITE,
    LEX_REAL,
    LEX_NULL
};

type_of_lex Scanner::dlms[] = {
    LEX_NULL,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_ASSIGN,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_EQ,
    LEX_LSS,
    LEX_GTR,
    LEX_PLUS,
    LEX_MINUS,
    LEX_UN_PLUS,
    LEX_UN_MINUS,
    LEX_TIMES,
    LEX_SLASH,
    LEX_PERCENT,
    LEX_LEQ,
    LEX_NEQ,
    LEX_GEQ,
    LEX_SQ_LPAREN,
    LEX_SQ_RPAREN,
    LEX_NULL
};

ostream& operator<< ( ostream &s, Lex l ) {
    int i;
    s << '(';
    if (l.t_lex==LEX_INT_NUM) {
        s << '"' << (int)l.v_lex << '"';
        goto E;
    }
    if (l.t_lex==LEX_REAL_NUM) {
        s << '"' << (double)l.v_lex << '"';
        goto E;
    }
    for (i=1; Scanner::words[i]; i++)
        if (Scanner::words[i]==l.t_lex) {
            s <<Scanner::TW[i];
            goto E;
        }
    for (i=1; Scanner::dlms[i]; i++)
        if (Scanner::dlms[i]==l.t_lex) {
            s << '"' << Scanner::TD[i] << '"';
            goto E;
        }
    if (l.t_lex>=LEX_ID) {
        s <<Scanner::TP[l.t_lex-LEX_ID];
        goto E;
    }
    s << l.t_lex;
E:  s << ',' << (int)l.v_lex << ");" << endl;
    return s;
}

Lex Scanner::get_lex () {
    int d=0, j;
    double r=0;
    CS = H;
    do {
        switch ( CS ) {
            case H:
                switch (c) {
                    case ' ': case '\n': case '\r': case '\t':
                        break;
                        
                    case '{':
                        CS=COM;
                        break;
                        
                    case '!':
                        clear ();
                        add ();
                        CS = NEQ;
                        break;
                        
                    case ':': case '<': case '>':
                        clear ();
                        add ();
                        CS = ALE;
                        break;
                        
                    case EOF:
                        return Lex(LEX_FIN);
                        
                    default:
                        if ( isalpha(c) || c=='_' ) {
                            clear ();
                            add ();
                            CS = IDENT;
                        }
                        else if ( isdigit(c) ) {
                            ungetc(c, fp);
                            if (fscanf(fp, "%d", &d)!=1)
                                throw "fscanf ERROR on integer part reading";
                            gc();
                            if (c=='.') {
                                ungetc(c, fp);
                                if (fscanf(fp, "%lf", &r)!=1)
                                    throw "fscanf ERROR fractional part reading";
                                gc();
                                return Lex ( LEX_REAL_NUM, d+r );
                            }
                            return Lex ( LEX_INT_NUM, d );
                        }
                        else {
                            CS = DELIM;
                            continue;
                        }
                        break;
                }
                break;
                
            case IDENT:
                if ( isalpha(c) || c=='_' || isdigit(c) )
                    add ();
                else
                    if ( (j = look (buf, TW)) )
                        return Lex (words[j], j);
                    else {
                        j = TID.put(buf);
                        return Lex (LEX_ID, j);
                    }
                break;
                
            case COM:
                switch (c) {
                    case '}':
                        CS=H;
                        break;
                        
                    case EOF:
                        return Lex(LEX_FIN);
                        break;
                }
                break;
                
            case ALE:
                switch (c) {
                    case '=':
                        add ();
                        gc ();
                        j = look ( buf, TD );
                        return Lex ( dlms[j], j );
                        
                    default:
                        j = look (buf, TD);
                        return Lex ( dlms[j], j );
                }
                break;
                
            case NEQ:
                switch (c) {
                    case '=':
                        add ();
                        gc ();
                        j = look ( buf, TD );
                        return Lex ( LEX_NEQ, j);
                        
                    default:
                        throw c;
                }
                break;
                
            case DELIM:
                clear ();
                add ();
                if ( (j = look(buf, TD)) ) {
                    gc ();
                    return Lex ( dlms[j], j );
                }
                else
                    throw c;
                break;
        }
        gc();
    }
    while ( true );
}

class Poliz {
    Lex *p;
    int size;
    int free;
    
public:
    Poliz ( int max_size ) {
        p = new Lex [size = max_size];
        free = 0;
    };
    
    ~Poliz() {
        delete [] p;
    };
    
    void put_lex(Lex l) {
        p[free]=l;
        ++free;
    };
    
    void put_lex(Lex l, int place) {
        p[place]=l;
    };
    
    void blank() {
        ++free;
    };
    
    int get_free() {
        return free;
    };
    
    Lex & operator[] ( int index ) {
        if (index > size)
            throw "POLIZ:out of array";
        else
            if ( index > free )
                throw "POLIZ:indefinite element of array";
            else
                return p[index];
    };
    
    void print() {
        for ( int i = 0; i < free; ++i )
            cout << i << ".\t" << p[i];
    };
};

class Parser {
    
    Lex curr_lex;                 // текущая лексема
    type_of_lex c_type;
    lex_val c_val;
    Scanner scan;
    Stack < lex_val, 100 > st_int;
    Stack < type_of_lex, 100 > st_lex;
    
    void P();                     // процедуры РС-метода
    void D1();
    void D();
    void X1();
    void X2();
    void B();
    void S();
    void E();
    void E1();
    void T();
    void F1();
    void F();
    
    void dec ( type_of_lex type); // семантичиеские действия
    void check_id ();
    void check_un_op ();
    void check_op ();
    void check_not ();
    void eq_type ();
    void eq_bool ();
    void check_id_in_read ();
    void gl () {                  // получить очередную лексему
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
        cout << curr_lex;
    }
    
public:
    Poliz prog;                   // внутреннее представление программы
    Parser ( const char *program) : scan (program), prog (1000) {}
    void analyze();               // анализатор с действиями
};

void Parser::analyze () {
    cout << "-------- LEXEMES: --------" << endl;
    gl ();
    P ();
    cout << "----- END OF LEXEMES -----" << endl;
    cout << endl << "--------- POLIZ: ---------" << endl;
    prog.print();
    cout << "------ END OF POLIZ ------" << endl;
}

void Parser::P () {
    if ( c_type == LEX_PROGRAM )
        gl ();
    else
        throw curr_lex;
    if ( c_type == LEX_ID )
        gl ();
    else
        throw curr_lex;
    if ( c_type == LEX_SEMICOLON )
        gl ();
    else
        throw curr_lex;
    D1 ();
    if ( c_type == LEX_SEMICOLON )
        gl ();
    else
        throw curr_lex;
    B ();
    if ( c_type != LEX_FIN )
        throw curr_lex;
}

void Parser::D1 () {
    if ( c_type == LEX_VAR ) {
        gl ();
        D ();
        while ( c_type == LEX_COMMA ) {
            gl();
            D();
        }
    }
    else
        throw curr_lex;
}

void Parser::D () {
    st_int.reset();
    X1();
    while (c_type == LEX_COMMA) {
        gl();
        X1();
    }
    if (c_type != LEX_COLON)
        throw curr_lex;
    else
    {
        gl();
        if (c_type == LEX_INT) {
            dec ( LEX_INT );
            gl();
        }
        else
            if (c_type == LEX_REAL) {
                dec ( LEX_REAL );
                gl();
            }
            else
                if (c_type == LEX_BOOL) {
                    dec ( LEX_BOOL );
                    gl();
                }
                else
                    throw curr_lex;
    }
}

void Parser::X1 () {
    if (c_type != LEX_ID)
        throw curr_lex;
    else {
        st_int.push ( c_val );
        gl();
    }
    if (c_type == LEX_SQ_LPAREN) {
        gl();
        if (c_type != LEX_INT_NUM)
            throw curr_lex;
        st_int.push ( c_val );
        gl();
        if (c_type != LEX_SQ_RPAREN)
            throw curr_lex;
        gl();
    }
    else {
        st_int.push ( lex_val(1) );
    }
}

void Parser::X2 () {
    if (c_type != LEX_ID)
        throw curr_lex;
    else {
        check_id();
        prog.put_lex (Lex(POLIZ_ADDRESS, (int) c_val));
        gl();
    }
    
    st_lex.push (LEX_INDEX);
    
    if (c_type == LEX_SQ_LPAREN) {
        gl();
        E();
        
        if (c_type != LEX_SQ_RPAREN)
            throw curr_lex;
        gl();
    }
    else {
        prog.put_lex (Lex(LEX_INT_NUM, 0));
        st_lex.push ( LEX_INT );
    }
}

void Parser::B () {
    if ( c_type == LEX_BEGIN ) {
        gl();
        S();
        while ( c_type == LEX_SEMICOLON ) {
            gl();
            S();
        }
        if ( c_type == LEX_END )
            gl();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}

void Parser::S () {
    int pl0, pl1, pl2, pl3;
    if ( c_type == LEX_IF ) {
        gl();
        E();
        eq_bool();
        pl2 = prog.get_free ();
        prog.blank();
        prog.put_lex (Lex(POLIZ_FGO));
        if ( c_type == LEX_THEN ) {
            gl();
            S();
            pl3 = prog.get_free();
            prog.blank();
            prog.put_lex (Lex(POLIZ_GO));
            prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()),pl2);
            if (c_type == LEX_ELSE) {
                gl();
                S();
            }
            prog.put_lex(Lex(POLIZ_LABEL,prog.get_free()),pl3);
        }
        else
            throw curr_lex;
    } //end if
    else
        if ( c_type == LEX_WHILE ) {
            pl0 = prog.get_free();
            gl();
            E();
            eq_bool();
            pl1 = prog.get_free();
            prog.blank();
            prog.put_lex (Lex(POLIZ_FGO));
            if ( c_type == LEX_DO ) {
                gl();
                S();
                prog.put_lex (Lex (POLIZ_LABEL, pl0));
                prog.put_lex (Lex (POLIZ_GO));
                prog.put_lex (Lex (POLIZ_LABEL, prog.get_free()),pl1);
            }
            else
                throw curr_lex;
        } //end while
        else
            if ( c_type == LEX_READ ) {
                gl();
                if ( c_type == LEX_LPAREN ) {
                    gl();
                    X2();
                    check_op();
                    st_lex.pop();
                    if ( c_type == LEX_RPAREN ) {
                        gl();
                        prog.put_lex (Lex (LEX_READ));
                    }
                    else
                        throw curr_lex;
                }
                else
                    throw curr_lex;
            } //end read
            else
                if ( c_type == LEX_WRITE ) {
                    gl();
                    if ( c_type == LEX_LPAREN ) {
                        gl();
                        E();
                        if ( c_type == LEX_RPAREN ) {
                            gl();
                            prog.put_lex (Lex(LEX_WRITE));
                        }
                        else
                            throw curr_lex;
                    }
                    else
                        throw curr_lex;
                } //end write
                else
                    if ( c_type == LEX_ID ) {
                        st_int.reset();
                        X2();
                        check_op();
                        if ( c_type == LEX_ASSIGN ) {
                            gl();
                            E();
                            eq_type();
                            prog.put_lex (Lex (LEX_ASSIGN) );
                        }
                        else
                            throw curr_lex;
                    } //assign-end
                    else
                        if ( c_type == LEX_BEGIN )
                            B();
}

void Parser::E () {
    E1();
    if ( c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
        c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
        st_lex.push (c_type);
        gl();
        E1();
        check_op();
    }
}

void Parser::E1 () {
    T();
    while ( c_type==LEX_PLUS || c_type==LEX_MINUS || c_type==LEX_OR ) {
        st_lex.push (c_type);
        gl();
        T();
        check_op();
    }
}

void Parser::T () {
    F1();
    while ( c_type==LEX_TIMES || c_type==LEX_SLASH || c_type==LEX_PERCENT || c_type==LEX_AND ) {
        st_lex.push (c_type);
        gl();
        F1();
        check_op();
    }
}

void Parser::F1 () {
    if ( c_type == LEX_PLUS ) {
        gl();
        F();
        check_un_op();
        prog.put_lex (Lex ( LEX_UN_PLUS ));
    } else
        if ( c_type == LEX_MINUS ) {
            gl();
            F();
            check_un_op();
            prog.put_lex (Lex ( LEX_UN_MINUS ));
        } else
            F();
}

void Parser::F () {
    if ( c_type == LEX_ID ) {
        X2();
        check_op();
        prog.put_lex (Lex(LEX_ID));
    }
    else
        if ( c_type == LEX_INT_NUM ) {
            st_lex.push ( LEX_INT );
            prog.put_lex ( curr_lex );
            gl();
        }
        else
            if ( c_type == LEX_REAL_NUM ) {
                st_lex.push ( LEX_REAL );
                prog.put_lex ( curr_lex );
                gl();
            }
            else
                if ( c_type == LEX_TRUE ) {
                    st_lex.push ( LEX_BOOL );
                    prog.put_lex (Lex (LEX_TRUE, 1) );
                    gl();
                }
                else
                    if ( c_type == LEX_FALSE ) {
                        st_lex.push ( LEX_BOOL );
                        prog.put_lex (Lex (LEX_FALSE, 0) );
                        gl();
                    }
                    else
                        if ( c_type == LEX_NOT ) {
                            gl();
                            F();
                            check_not(); }
                        else
                            if ( c_type == LEX_LPAREN ) {
                                gl();
                                E();
                                if ( c_type == LEX_RPAREN)
                                    gl();
                                else
                                    throw curr_lex;
                            }
                            else
                                throw curr_lex;
}

void Parser::dec ( type_of_lex type ) {
    int i, dim;
    while ( !st_int.is_empty()) {
        dim = st_int.pop();
        i = st_int.pop();
        if ( TID[i].get_declare() )
            throw "twice";
        if ( dim <= 0 )
            throw "wrong size of array";
        else {
            TID[i].put_declare();
            TID[i].put_dim(dim);
            TID[i].put_type(type);
        }
    }
}

void Parser::check_id() {
    if ( TID[c_val].get_declare() ) {
        st_lex.push(TID[c_val].get_type());
    }
    else
        throw "not declared";
}

void Parser::check_id_in_read () {
    if ( !TID[c_val].get_declare() )
        throw "not declared";
}

void Parser::check_un_op () {
    type_of_lex t1;
    t1 = st_lex.pop();
    
    if (t1!=LEX_INT && t1!=LEX_REAL) {
        throw "wrong type is in operation ( unary + | unary - )";
    }
    
    st_lex.push(t1);
}

void Parser::check_op () {
    type_of_lex t1, t2, op, t, r;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    switch (op) {
        case LEX_PERCENT:
            t=r=LEX_INT;
            if (t1!=t || t2!=t)
                throw "wrong types are in operation ( % )";
            st_lex.push(r);
            prog.put_lex (Lex (op) );
            return;
            
        case LEX_INDEX:
            r=t1;
            t=LEX_INT;
            if (t2 == t)
                st_lex.push(r);
            else
                throw "wrong index expression type";
            return;
            
        case LEX_PLUS: case LEX_MINUS: case LEX_TIMES: case LEX_SLASH:
            r=LEX_INT;
            if (t1==LEX_REAL || t2 == LEX_REAL)
                r=LEX_REAL;
            if ((t1!=LEX_REAL && t1!=LEX_INT) || (t2!=LEX_REAL && t2!=LEX_INT))
                throw "wrong types are in operation ( + | - | * | / )";
            st_lex.push(r);
            prog.put_lex (Lex (op) );
            return;
            
        case LEX_OR: case LEX_AND:
            t=r=LEX_BOOL;
            if (t1!=t || t2!=t)
                throw "wrong types are in operation ( and | or )";
            st_lex.push(r);
            prog.put_lex (Lex (op) );
            return;
            
        case LEX_EQ: case LEX_NEQ:
            r=LEX_BOOL;
            if ((t1==LEX_BOOL && t2!=LEX_BOOL) || (t2==LEX_BOOL && t1!=LEX_BOOL))
                throw "wrong types are in operation ( = | != )";
            st_lex.push(r);
            prog.put_lex (Lex (op) );
            return;
            
        default:
            r=LEX_BOOL;
            if ((t1!=LEX_REAL && t1!=LEX_INT) || (t2!=LEX_REAL && t2!=LEX_INT))
                throw "wrong types are in operation ( > | < | >= | <= )";
            st_lex.push(r);
            prog.put_lex (Lex (op) );
            return;
    }
}

void Parser::check_not () {
    if (st_lex.pop() != LEX_BOOL)
        throw "wrong type is in operation ( not )";
    else {
        st_lex.push (LEX_BOOL);
    }
    prog.put_lex (Lex (LEX_NOT) );
}

void Parser::eq_type () {
    type_of_lex t1, t2;
    t2 = st_lex.pop();
    t1 = st_lex.pop();
    if ( (t1 != t2) && !(t1==LEX_REAL && t2==LEX_INT) )
        throw "wrong types are in operation ( := )";
}

void Parser::eq_bool () {
    if ( st_lex.pop() != LEX_BOOL )
        throw "expression is not boolean";
}

class Executer {
    Lex pc_el;
    
public:
    void execute ( Poliz& prog );
};

void Executer::execute ( Poliz& prog ) {
    Stack < lex_val, 100 > args;
    int index = 0, size = prog.get_free();
    lex_val i,j, dim;
    
    cout << endl << "--- PROGRAM EXECUTING: ---" << endl;
    while ( index < size ) {
        pc_el = prog [ index ];
        switch ( pc_el.get_type () ) {
            case LEX_TRUE:
            case LEX_FALSE:
            case LEX_INT_NUM:
            case LEX_REAL_NUM:
            case POLIZ_ADDRESS:
            case POLIZ_LABEL:
                args.push ( pc_el.get_value () );
                break;
            case LEX_ID:
                j = args.pop();
                i = args.pop();
                if ( TID[i].get_assign(j) ) {
                    args.push ( TID[i].get_value (j) );
                    break;
                }
                else
                    throw "POLIZ: indefinite identifier";
            case LEX_NOT:
                args.push( lex_val(!args.pop()) );
                break;
            case LEX_OR:
                i = args.pop();
                args.push ( args.pop() || i );
                break;
            case LEX_AND:
                i = args.pop();
                args.push ( args.pop() && i );
                break;
            case POLIZ_GO:
                index = (int)args.pop() - 1;
                break;
            case POLIZ_FGO:
                i = args.pop();
                if ( !args.pop() )
                    index = (int)i - 1;
                break;
            case LEX_WRITE:
                i = args.pop ();
                if (i.is_real())
                    cout << (double) i << endl;
                else
                    cout << (int) i << endl;
                break;
            case LEX_READ: {
                dim = args.pop();
                i = args.pop();
                if ( TID[i].get_type () == LEX_INT ) {
                    int k;
                    cin >> k;
                    TID[i].put_value (dim,k);
                }
                else
                    if ( TID[i].get_type () == LEX_REAL ) {
                        double k;
                        cin >> k;
                        TID[i].put_value (dim,k);
                    }
                    else
                    {
                        char j[20];
                        int k;
                    rep:
                        cin >> j;
                        if ( !strcmp(j, "true") )
                            k = 1;
                        else if ( !strcmp(j, "false") )
                            k = 0;
                        else {
                            cout << "Error in input:true/false"; cout << endl;
                            goto rep;
                        }
                        TID[i].put_value (dim,k);
                    }
                TID[i].put_assign (dim);
                break;
            }
            case LEX_UN_PLUS:
                args.push( lex_val(args.pop()) );
                break;
            case LEX_UN_MINUS:
                i = args.pop ();
                if (i.is_real())
                    args.push( lex_val(-(double)i) );
                else
                    args.push( lex_val(-(int)i) );
                break;
            case LEX_PLUS:
                i=args.pop();
                args.push ( args.pop() + i );
                break;
            case LEX_TIMES:
                i=args.pop();
                args.push ( args.pop() * i );
                break;
            case LEX_MINUS:
                i=args.pop();
                args.push ( args.pop() - i );
                break;
            case LEX_SLASH:
                i = args.pop();
                if ( i.is_real()?(double)i:(int)i ) {
                    args.push ( args.pop() / i );
                    break;
                }
                else
                    throw "POLIZ:divide by zero";
            case LEX_PERCENT:
                i = args.pop();
                if ( i.is_real()?(double)i:(int)i ) {
                    args.push ( args.pop() % i );
                    break;
                }
                else
                    throw "POLIZ:divide by zero";
            case LEX_EQ:
                i=args.pop();
                args.push ( args.pop() == i );
                break;
            case LEX_LSS:
                i=args.pop();
                args.push ( args.pop() < i );
                break;
            case LEX_GTR:
                i=args.pop();
                args.push ( args.pop() > i );
                break;
            case LEX_LEQ:
                i=args.pop();
                args.push ( args.pop() <= i );
                break;
            case LEX_GEQ:
                i=args.pop();
                args.push ( args.pop() >= i );
                break;
            case LEX_NEQ:
                i=args.pop();
                args.push ( args.pop() != i );
                break;
            case LEX_ASSIGN:
                i = args.pop();
                dim = args.pop();
                j = args.pop();
                if (i.is_real())
                    TID[j].put_value(dim, (double)i);
                else
                    TID[j].put_value(dim, (int)i);
                TID[j].put_assign(dim);
                break;
            default:
                throw "POLIZ: unexpected elem";
        } // end of switch
        ++index;
    }  //end of while
    cout << "-- FINISH OF EXECUTING. --" << endl;
}

class Interpretator {
    Parser pars;
    Executer E;
public:
    Interpretator ( char* program ): pars (program) {};
    void interpretation () {
        pars.analyze ();
        E.execute ( pars.prog );
    }
};

int main(int argc, char **argv) {
    if (argc==1) {
        cout << "It's not enough arguments!!!" << endl;
        return -1;
    }
    
    try {
        Interpretator I (argv[1]);
        I.interpretation ();
        return 0;
    }
    
    catch (const char *str) {
        cout << str << endl;
    }
    catch (int i) {
        char c=i;
        cout << "ERROR!!! Unexpected symbol " << '"' << c << '"' << endl;
    }
    catch ( Lex l ) {
        cout << "Unexpected lexeme " << l;
    }
    catch (...) {
        cout << "Unidentified ERROR!!!" << endl;
    }
    
    return -1;
}
