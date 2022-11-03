"#set tgc

let &t_SI = "\<Esc>]50;CursorShape=1\x7"
let &t_SR = "\<Esc>]50;CursorShape=2\x7"
let &t_EI = "\<Esc>]50;CursorShape=0\x7"

set ts=4 sw=4

set nu
"set guicursor=i:block
set guicursor=i:blinkon5
set guicursor+=n:ver5-Cursor
"set guicursor=v-c:blinkon5
"set guicursor+=c:hor10-Cursor/lCursor

highlight LineNr term=bold cterm=NONE ctermfg=DarkGray ctermbg=NONE gui=NONE
highlight CursorLineNr term=bold cterm=NONE ctermfg=White ctermbg=DarkGray cterm=NONE 
highlight CursorLine term=bold cterm=bold ctermbg=DarkGray


set cursorline
"set cursorcolumn
"set cursorlineopt=number

"hi CursorLineNr cterm=bold
"set cursorline
"set cursorlineopt=number 

set completeopt=menuone,longest
set mouse=a
set title
set wildmenu

" automatic syntax detection and highlighting based on the file type 
filetype plugin indent on
syntax on

" support more colors
set t_Co=256

" italics
let &t_ZH="\e[3m"
let &t_ZR="\e[23g"

function! ShowColourSchemeName()
    try
        echo g:colors_name
    catch /^Vim:E121/
        echo "default"
    endtry
endfunction


