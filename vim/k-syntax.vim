" vim syntax file by kcm1700
" for k- language

if exists ("b:current_syntax")
  finish
endif

syn match   kmParenError ")"
syn match   kmParenError "}"
syn match   kmParenError "\*)"

syn region  kmParen start="(" end=")" contains=kmKeyword,kmNumber,kmOperator,kmParen,kmBrace,kmComment
syn region  kmBrace start="{" end="}" contains=kmKeyword,kmNumber,kmOperator,kmParen,kmBrace,kmComment
syn region  kmComment start="(\*" end="\*)"

syn keyword kmKeyword unit true false not if then else let in proc while do read write
syn match   kmNumber        "\<-\?\d\(\d\)*\>"
syn match   kmOperator "\(:=\|+\|-\|*\|\/\|<\|=\)"


hi link kmOperator Keyword
hi link kmKeyword Keyword
hi link kmNumber Number
hi link kmComment Comment
hi link kmParenError Error
