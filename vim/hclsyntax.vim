" vim syntax file by kcm1700
" for hcl language

if exists ("b:current_syntax")
  finish
endif

syn match   kmParenError ")"
syn match   kmParenError "}"
syn match   kmParenError "\]"

syn region  kmParen start="(" end=")" contains=kmKeyword,kmOperator,kmParen,kmBrace,kmSqBrace,kmComment
syn region  kmBrace start="{" end="}" contains=kmKeyword,kmOperator,kmParen,kmBrace,kmSqBrace,kmComment
syn region  kmSqBrace start="\[" end="\]" contains=kmKeyword,kmNumber,kmOperator,kmParen,kmBrace,kmComment
syn region  kmComment start="#" end="$"

syn keyword kmKeyword int bool intsig boolsig in quote
syn match   kmQuoteString "'.*'"
syn match   kmOperator "\(:\|&&\|||\|,\|!\|=\)"

hi link kmOperator Keyword
hi link kmKeyword Keyword
hi link kmComment Comment
hi link kmParenError Error
hi link kmQuoteString String
