# C-Doku

Sudoku, in C. Its a lame pun, I know. Anyway, building and executing is very simple.

### Build

1. `make NAME=<name of executable>`
2. `./<name of executable>`
3. (optional) `make clean` to clean up leftover build files

### Quick Start

For control information, run the executable with the `--help` argument.

To get started playing sudoku with a partially filled board, run the executable with any of the following strings:

* `dddd3dd2daxd2d1s3s8a7aw9saaa1a6a3a2sd1d5d6d2ddd3s6s1s4d5aa2a8w3d7aa4dw1awas8aa2s6s7a6s1s8d5wd2dsd7`
* `dd1asa4ddd1dw5dd8dd34s6aa2s9a4a2s6d1d4sd2d5as9dss9saa5aa9dw3wa1a4a9s2s3aa8aw1www9d727w2ssd4d5d7`
* `d141d5dddd4d9sd8aaaad1a5a3saaa7ddddd2sd8s2s9dw3sd6s1ass8a5wa5a7a8awd9wd8d3wwaa1a6ddda1a6a2aa3ssdw6d8s4asas9sd2d7`
* `d5d1d2dddd7sd8saa1a6a5w3aa7aa4s8ss2dd3wd8d9dd3s6dw5ds1ss6s5as1a8w2sa3aw1w2w6d1aaa8a7sd5d9saaa9sd6q`

Feel free to create a pull request to add your own action strings. You can generate them by running the executable with the `--record` flag. At this time, action strings are unsupported for recorded games.

### Action Strings

An "action string" is a sequence of ASCII characters which the program will execute as if it were input by the user. An action string is **not case sensitive**. The following characters are valid in such a string:

    - **Any digit (1-9)**: places the digit in the current cell
    - **WASD**: Move up/left/down/right respectively
    - **HJKL**: Move left/up/down/right respectively
    - **X**: Clear cell
    - **Q**: Quit

You may generate an action string by starting an empty game with the `--record` argument, or by hand-writing them using the above encoding. In the former, a file named `recording.txt` will be output in your working directory containing the string.
