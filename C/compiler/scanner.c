/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: scanner.c
 * Autori:  Kollar David <xkolla07>
 *          Masarova Maria <xmasar13>
 *          Pristas Jan <xprist06>
 *          Yaremchuk Roman <xyarem00>
 */

#include "scanner.h"


FILE *source;

const char *token_names[]=
{
    "and",
    "boolean",
    "continue",
    "elseif",
    "exit",
    "false",
    "for",
    "next",
    "not",
    "or",
    "shared",
    "static",
    "true",
    //---------Rezervovaná klícová slova
    "as",
    "asc",
    "declare",
    "dim",
    "do",
    "double",
    "else",
    "end",//!!!!!!!!!!!!
    "chr",
    "function",
    "if",
    "input",
    "integer",
    "lenght",
    "loop",
    "print",
    "return",
    "scope",
    "string",
    "substr",
    "then",
    "while"
};

void setSourceFile(FILE *f)
{
  source = f;
}

int UpperToLower(int c){
    if(c >= 65 && c<= 90)
        c = c + 32;
    return c;
}

SToken GetToken()
{
    States state;
    state = state_start;

    SToken new_token;
    
    string str;
    InitString(&str);
    ClearString(&str);

    int c;
    int c_end=0;

    while(1)
    {
        c = fgetc(source);
        UpperToLower(c);
        PrintString(&str);
        switch(state)
            {
            case state_start:

                switch(c)// + - / < > =
                {
                    case '\\': // MOD 
                        new_token.type = token_mod;
                        return new_token;

                    case '\'': 
                        state = state_comment_line;
                        break;

                    case ',':
                        new_token.type = token_colom;
                        return new_token;

                    case '\0':
                        state = state_start;
                        break;

                    case ';':
                        new_token.type=token_semicolom;
                        return new_token;

                    case '+':
                        new_token.type=token_plus;
                        return new_token;

                    case '-':
                        new_token.type=token_minus;
                        return new_token;

                    case '*':
                        new_token.type=token_mul;
                        return new_token;

                    case '/':
                        state = state_comment;
                        c = fgetc(source);
                        if (c == '\''){
                            state = state_comment;
                            break;
                        } else {
                            ungetc(c, source);
                            new_token.type = token_slash;
                            return new_token;
                        }
                        break;

                    case '!':
                        state=state_array;
                        break;

                    case '=':
                        new_token.type=token_assignment;
                        return new_token;

                    case '(':
                        new_token.type=token_leftBracket;
                        return new_token;

                    case ')':
                        new_token.type=token_rightBracket;
                        return new_token;

                    case '>':
                        state=state_greater;
                        break;

                    case '<':
                        state=state_less;
                        break;

                    case EOF:
                        new_token.type = token_eof;
                        return new_token;

                    case '\n':
                        new_token.type = token_eol;
                        return new_token;

                    default: //id nebo int
                        if (isalpha(c) || c =='_')
                        {
                            c = UpperToLower(c);
                            state = state_id;
                            AddCharToString(&str, c);

                        } else if (isdigit(c))
                        {
                            state = state_int;
                            AddCharToString(&str, c);
                        }else if (isspace(c)){
                            break;
                        }
                        else
                        {
                            state = state_error;
                        break;
                        }
                }
                break;

            case state_id:
                if((isdigit(c)) || (isalpha(c)) || (c == '_'))
                {
                    c = UpperToLower(c);
                    AddCharToString(&str, c);
                    state=state_id;
                } else if (c == '.'){
                    AddCharToString(&str, c);
                    state=state_int;
                } else if (c == ';'){
                    ungetc(c, source);
                    new_token.name = malloc(sizeof(char)*(strlen((char *)&str)+ 1));
                    new_token.name = GetString(&str);
                    new_token.type = token_id;
                    return new_token;
                } else if (c == '('){
                    ungetc(c, source);
                    new_token.name = malloc(sizeof(char)*(strlen((char *)&str)+ 1));
                    new_token.name = GetString(&str);
                    new_token.type = token_id;
                    return new_token;
                }
                else
                {   
                    if ((c == ',') || (c == ')')){
                        ungetc(c, source);
                        if ((!CompareStringToChar(&str, "double")) && (!CompareStringToChar(&str, "string")) && (!CompareStringToChar(&str, "integer"))){
                            new_token.name = malloc(sizeof(char)*(strlen((char *)&str)+ 1));
                            new_token.name = GetString(&str);
                            new_token.type = token_id;
                            return new_token;
                        }
                    }
          
                    if(c == 10)
                    {
                        ungetc(c, source);
                         if(CompareStringToChar(&str, "if"))
                        {   
                            new_token.type = token_if;
                            return new_token;
                        } else if(CompareStringToChar(&str, "scope"))
                        {
                            if (c_end == 0){
                                new_token.type = token_scope;
                            } else {
                                new_token.type = token_end_scope;
                            }
                            return new_token;
                        } else if(CompareStringToChar(&str, "function"))
                        {
                            new_token.type = token_function;
                            return new_token;
                        } else if(CompareStringToChar(&str, "endscope"))
                        {
                            c = fgetc(source);
                            new_token.type=token_end_scope;
                            return new_token;
                        } else if (CompareStringToChar(&str, "endfunction")){
                            new_token.type=token_end_function;
                            return new_token;
                        } else if (CompareStringToChar(&str, "endif")){
                            new_token.type=token_end_if;
                            return new_token;
                        } else
                        {
                            state=state_error;
                        }
                    }

                    if(CompareStringToChar(&str, "end"))
                    {
                        c_end = 1;
                        state = state_start;
                        break;
                    }

                    if(CompareStringToChar(&str, "and"))
                    {
                        new_token.type=token_and;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "boolean"))
                    {
                        new_token.type=token_boolean;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "continue"))
                    {
                        new_token.type=token_continue;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "elseif"))
                    {
                        new_token.type=token_else_if;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "exit"))
                    {
                        new_token.type=token_exit;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "false"))
                    {
                        new_token.type=token_false;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "for"))
                    {
                        new_token.type=token_for;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "or"))
                    {
                        new_token.type=token_or;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "shared"))
                    {
                        new_token.type=token_shared;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "static"))
                    {
                        new_token.type=token_static;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "true"))
                    {
                        new_token.type=token_true;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "as"))
                    {
                        new_token.type=token_as;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "asc"))
                    {
                        new_token.type=token_asc;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "declare"))
                    {
                        new_token.type=token_declare;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "dim"))
                    {
                        new_token.type=token_dim;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "do"))
                    {
                        new_token.type=token_do;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "double"))
                    {
                        new_token.type=token_double;
                        //ungetc(c, source);
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "else"))
                    {
                        new_token.type=token_else;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "integer"))
                    {
                        new_token.type = token_integer;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "function"))//! END?
                    {
                        new_token.type=token_function;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "if"))
                    {
                        new_token.type=token_if;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "input"))
                    {
                        new_token.type=token_input;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "lenght"))
                    {
                        new_token.type=token_lenght;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "loop"))
                    {
                        new_token.type=token_loop;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "print"))
                    {
                        new_token.type=token_print;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "return"))
                    {
                        new_token.type=token_return;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "scope"))// END??
                    {
                        new_token.type=token_scope;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "string"))
                    {
                        new_token.type=token_string;
                        //ungetc(c, source);
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "substr"))
                    {
                        new_token.type=token_substr;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "then"))
                    {
                        new_token.type=token_then;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "while"))
                    {
                        new_token.type=token_while;
                        return new_token;
                    }

                    if(CompareStringToChar(&str, "endscope"))
                    {
                        new_token.type=token_end_scope;
                        return new_token;
                    } else if (CompareStringToChar(&str, "endfunction")){
                        new_token.type=token_end_function;
                        return new_token;
                    } else if (CompareStringToChar(&str, "endif")){
                        new_token.type=token_end_if;
                        return new_token;
                    }

                    if (isspace(c))
                    {
                        new_token.name = malloc(sizeof(char)*(strlen((char *)&str)+ 1));
                        new_token.name = GetString(&str);
                        new_token.type=token_id;
                        if (c_end == 1){
                            exit(2);
                        }
                        return new_token;
                    }

                    if (c == 10){
                        ungetc(c, source);
                        new_token.name = malloc(sizeof(char)*(strlen((char *)&str)+ 1));
                        new_token.name = GetString(&str);
                        new_token.type=token_id;
                        if (c_end == 1){
                            exit(2);
                        }
                        return new_token;
                    }

                    if (c_end == 1){
                        exit(2);
                    }
                    
                    state = state_error;
                }
                break;

                case state_int:
                        if(isdigit(c))
                        {
                            AddCharToString(&str, c);
                            state = state_int;
                            break;
                        } else if ((c == 'e') || (c =='E'))
                        {
                            AddCharToString(&str, c);
                            state=state_exponent;
                            break;
                        } else if (c == '.')
                        {
                            AddCharToString(&str, c);
                            state=state_double;
                            break;
                        } else if (isalpha(c)) {
                            state = state_error;
                            break;
                        } else if (c == '\n' || c == ';'){
                        	ungetc(c, source);
                        	new_token.type = token_val;
                        	return new_token;
                        } else if (c == ')'){
                            ungetc(c, source);
                            new_token.type = token_val;
                            return new_token;
                        } else if (c == ',') { 
                            ungetc(c, source);
                            new_token.type = token_val;
                            new_token.var_type = 1;
                            new_token.value.number= atoi(GetString(&str));
                            return new_token;
                        } else {
                            new_token.type = token_val;
                            new_token.var_type = 0;
                            new_token.value.number = atoi(GetString(&str));
                            return new_token;
                        }
                    break;

           case state_double:
                        if(isdigit(c))
                        {
                            AddCharToString(&str, c);
                            state = state_double1;
                            break;
                        } else state = state_error;
                    break;

            case state_double1:
                        if (isdigit(c))
                        {
                            AddCharToString(&str, c);
                            state = state_double1;
                            break;
                        }
                        if ((c == 'e') || (c == 'E'))
                        {
                            AddCharToString(&str, c);
                            state = state_exponent;
                            break;
                        } else if (c <= 32){
                            new_token.type = token_val;
                            new_token.var_type = 1;
                            new_token.value.number_double= atof(GetString(&str));
                            return new_token;
                        } else if (c == ',') { 
                            ungetc(c, source);
                            new_token.type = token_val;
                            new_token.var_type = 1;
                            new_token.value.number_double= atof(GetString(&str));
                            return new_token;
                        } else {
                            state = state_error;
                            break;
                        }
                    break;

            case state_exponent:
                        if ((c == '+') || (c == '-'))
                        {
                            AddCharToString(&str, c);
                            state = state_exponent2;
                            break;
                        } else if (isdigit(c))
                        {
                            AddCharToString(&str, c);
                            state = state_exponent3;
                            break;
                        } else state = state_error;
                    break;

            case state_exponent2:
                        if (isdigit(c))
                        {
                            AddCharToString(&str, c);
                            state = state_exponent3;
                            break;
                        } else state = state_error;
                    break;

            case state_exponent3:
                        if (isdigit(c))
                        {
                            AddCharToString(&str, c);
                            state = state_exponent3;
                            break;
                        } else if ((c == 32) || (c == '\n') || (c == '\t')) {
                            new_token.type = token_val;
                            new_token.var_type = 1;
                            new_token.value.number_double = atof(GetString(&str));
                            return new_token;
                        } else {
                            state = state_error;
                            break;
                        }
                    break;

            case state_greater:// <= >= <>
                        if (c == '=') ///i+1
                        {
                            new_token.type=token_greaOrEq;
                            return new_token;
                        } else
                        {
                            new_token.type=token_greater;
                            return new_token;
                        }
                    break;
            case state_less:
                        if (c == '>')
                        {
                            new_token.type=token_notEqual;
                            return new_token;
                        } else if (c == '=')
                        {
                            new_token.type=token_lessOrEq;
                            return new_token;
                        } else {
                            new_token.type=token_less;
                            return new_token;
                        }
                    break;

            case state_end:
                       
                    break;

            case state_comment:
                if (c == '\'') {
                    state = state_comment1; 
                } else if (c == EOF) {
                    state = state_error;
                    break;
                }
                break;

            case state_comment1:
                       if (c == '/') {
                            state = state_start;
                            break;
                       } else {
                            state = state_comment;
                            break;
                       }

            case state_comment_line:
                        if (c == '\n')
                        {
                            ungetc(c, source);
                            state = state_start;
                        }
                    break;

            case state_array:
                    if (c == '"')
                    {
                        state = state_string;
                    }
                    else state = state_error;
                break;

            case state_string:
                if (c == 92){
                    state = state_escape;
                    break;
                } else if (c == '"') {
                    new_token.type = token_string;
                    new_token.var_type = 2;
                    new_token.value.array=malloc(sizeof(char)*(strlen(GetString(&str)) + 1));
                    strcpy(new_token.value.array, GetString(&str));
                    return new_token;

                } else if (c == EOF) {
                    state = state_error;
                    break;
                } else {
                    AddCharToString(&str, c);
                }
                break;

            case state_escape:
                AddCharToString(&str, c);
                if (c == '"'){
                    state = state_string;
                    break;
                } else if (c == '\\') {
                    state = state_string;
                    break;
                } else if (c == 't'){
                    state = state_string;
                    break;
                } else if (c == 'n'){
                    state = state_string;
                    break;
                } else if (isdigit(c)) {
                    if (c == '0'){
                        c = fgetc(source);
                        if ((isdigit(c)) && (c == '0')){
                            c = fgetc(source);
                            if (isdigit(c) && c != '0'){
                                state = state_string;
                                break;
                            } else {
                                state = state_error;
                                break;
                            }
                        } else if (isdigit(c)){
                            c = fgetc(source);
                            if (isdigit(c)){
                                state = state_string;
                                break;
                            } else {
                                state = state_error;
                                break;
                            }
                        } else {
                            state = state_error;
                            break;
                        }
                    } else if (c == '1'){
                        c = fgetc(source);
                        if (isdigit(c)){
                            c = fgetc(source);
                            if (isdigit(c)){
                                state = state_string;
                                break;
                            } else {
                                state = state_error;
                                break;
                            }
                        } else {
                            state = state_error;
                            break;
                        }
                    } else if (c == '2'){
                        c = fgetc(source);
                        if (isdigit(c) && (c != '6') && (c != '7') && (c != '8') && (c != '9')){
                            if (isdigit(c) && c != '5'){
                                c = fgetc(source);
                                if (isdigit(c)) {
                                    state = state_string;
                                    break;
                                } else {
                                    state = state_error;
                                    break;
                                }
                            } else if (isdigit(c) && c == '5') {
                                c = fgetc(source);
                                if (isdigit(c) && (c != '6') && (c != '7') && (c != '8') && (c != '9')){
                                    state = state_string;
                                    break;
                                } else {
                                    state = state_error;
                                    break;
                                }
                            } else {
                                state = state_error;
                                break;
                            }

                        } else {
                            state = state_error;
                            break;
                        }
                    }
                    break;
                } else {
                    state = state_error;
                    break;
                }
                

            case state_error:
                FreeString(&str);
                //error_print(ELEX);
                exit(ELEX);
                break;

        }

    }

    if (c == EOF){
        new_token.type = token_eof;
        return new_token;            
    }

    return new_token;
}
