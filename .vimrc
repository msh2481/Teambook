set mouse=a
set nocp
set sc
set nu
set t_Co=256
set tgc
set noswf
set ts=4
set sw=4
set ai
set si
colo slate
set ul=1000000
set clipboard=unnamed

map cp :w<cr>ggVGy
map <f5> :w<cr>:!time ./%:r<cr>
map <f8> :w<cr>:!time g++ -std=c++14 -O3 -DLC -o %:r %<cr>
map <f9> :w<cr>:!time g++ -std=c++14 -O2 -DLC -Wall -Wextra -o %:r %<cr>
map <f10> :w<cr>:!time g++ -std=c++14 -O2 -DLC -Wall -Wextra -Wconversion -Wshadow -o %:r %<cr>
