/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    FOR = 260,                     /* FOR  */
    WHILE = 261,                   /* WHILE  */
    ID = 262,                      /* ID  */
    LPAREN = 263,                  /* LPAREN  */
    RPAREN = 264,                  /* RPAREN  */
    SEMICOLON = 265,               /* SEMICOLON  */
    LCURL = 266,                   /* LCURL  */
    RCURL = 267,                   /* RCURL  */
    COMMA = 268,                   /* COMMA  */
    INT = 269,                     /* INT  */
    FLOAT = 270,                   /* FLOAT  */
    VOID = 271,                    /* VOID  */
    LTHIRD = 272,                  /* LTHIRD  */
    CONST_INT = 273,               /* CONST_INT  */
    RTHIRD = 274,                  /* RTHIRD  */
    PRINTLN = 275,                 /* PRINTLN  */
    RETURN = 276,                  /* RETURN  */
    ASSIGNOP = 277,                /* ASSIGNOP  */
    LOGICOP = 278,                 /* LOGICOP  */
    RELOP = 279,                   /* RELOP  */
    ADDOP = 280,                   /* ADDOP  */
    MULOP = 281,                   /* MULOP  */
    CONST_FLOAT = 282,             /* CONST_FLOAT  */
    NOT = 283,                     /* NOT  */
    INCOP = 284,                   /* INCOP  */
    DECOP = 285,                   /* DECOP  */
    LOWER_THAN_ELSE = 286          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define FOR 260
#define WHILE 261
#define ID 262
#define LPAREN 263
#define RPAREN 264
#define SEMICOLON 265
#define LCURL 266
#define RCURL 267
#define COMMA 268
#define INT 269
#define FLOAT 270
#define VOID 271
#define LTHIRD 272
#define CONST_INT 273
#define RTHIRD 274
#define PRINTLN 275
#define RETURN 276
#define ASSIGNOP 277
#define LOGICOP 278
#define RELOP 279
#define ADDOP 280
#define MULOP 281
#define CONST_FLOAT 282
#define NOT 283
#define INCOP 284
#define DECOP 285
#define LOWER_THAN_ELSE 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 747 "2005046.y"

	SymbolInfo* sym;

#line 133 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
