                ;;      zx6x8.s
                ;;      zx6x8_font.s
                ;; 
                ;;      notes: see font.h for format details
                .module zx6x8_font

                .globl zx6x8_font

                .area _CODE
zx6x8_font::
                ;; font header
                .db 17                  ; font generation
                .db 6                   ; font width (max width for proportional)
                .db 8                   ; font height
                .db 1                   ; line width in bytes (unused for tiny)
                .db 32                  ; first ascii
                .db 127                 ; last ascii

                ;; ascii 32: ' '
                .db 0                   ; # moves
                ;; ascii 33: '!'
                .db 5                   ; # moves
                .db 3                   ; origin x
                .db 1                   ; origin y
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 6                   ; pen up
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 34: '"'
                .db 6                   ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 6                   ; pen up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 35: '#'
                .db 17                  ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 134                 ; move dx=0, dy=0, direction=left
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 6                   ; pen up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 36: '$'
                .db 18                  ; # moves
                .db 5                   ; origin x
                .db 2                   ; origin y
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 6                   ; pen up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 7                   ; pen down, point set
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 134                 ; move dx=0, dy=0, direction=left
                .db 6                   ; pen up
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 37: '%'
                .db 14                  ; # moves
                .db 1                   ; origin x
                .db 6                   ; origin y
                .db 161                 ; move dx=1, dy=0, direction=rigt up
                .db 6                   ; pen up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 7                   ; pen down, point set
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                ;; ascii 38: '&'
                .db 14                  ; # moves
                .db 5                   ; origin x
                .db 4                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 209                 ; move dx=2, dy=2, direction=rigt up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 6                   ; pen up
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 7                   ; pen down, point set
                .db 213                 ; move dx=2, dy=2, direction=right down
                ;; ascii 39: '''
                .db 1                   ; # moves
                .db 3                   ; origin x
                .db 1                   ; origin y
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 40: '('
                .db 3                   ; # moves
                .db 4                   ; origin x
                .db 1                   ; origin y
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                ;; ascii 41: ')'
                .db 3                   ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 42: '*'
                .db 12                  ; # moves
                .db 4                   ; origin x
                .db 2                   ; origin y
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 173                 ; move dx=1, dy=1, direction=right down
                ;; ascii 43: '+'
                .db 9                   ; # moves
                .db 3                   ; origin x
                .db 2                   ; origin y
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 44: ','
                .db 2                   ; # moves
                .db 3                   ; origin x
                .db 5                   ; origin y
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 45: '-'
                .db 1                   ; # moves
                .db 1                   ; origin x
                .db 4                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 46: '.'
                .db 3                   ; # moves
                .db 2                   ; origin x
                .db 5                   ; origin y
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                ;; ascii 47: '/'
                .db 1                   ; # moves
                .db 5                   ; origin x
                .db 2                   ; origin y
                .db 167                 ; move dx=1, dy=0, direction=left down
                ;; ascii 48: '0'
                .db 13                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 154                 ; move dx=0, dy=3, direction=up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 49: '1'
                .db 7                   ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 50: '2'
                .db 9                   ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 51: '3'
                .db 15                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 6                   ; pen up
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 52: '4'
                .db 5                   ; # moves
                .db 4                   ; origin x
                .db 4                   ; origin y
                .db 154                 ; move dx=0, dy=3, direction=up
                .db 255                 ; move dx=3, dy=3, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 53: '5'
                .db 12                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 230                 ; move dx=3, dy=0, direction=left
                .db 146                 ; move dx=0, dy=2, direction=up
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 54: '6'
                .db 11                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 55: '7'
                .db 4                   ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 255                 ; move dx=3, dy=3, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 56: '8'
                .db 14                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 57: '9'
                .db 11                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 154                 ; move dx=0, dy=3, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 58: ':'
                .db 6                   ; # moves
                .db 3                   ; origin x
                .db 3                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 59: ';'
                .db 7                   ; # moves
                .db 3                   ; origin x
                .db 2                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 60: '<'
                .db 2                   ; # moves
                .db 4                   ; origin x
                .db 2                   ; origin y
                .db 215                 ; move dx=2, dy=2, direction=left down
                .db 213                 ; move dx=2, dy=2, direction=right down
                ;; ascii 61: '='
                .db 5                   ; # moves
                .db 5                   ; origin x
                .db 3                   ; origin y
                .db 230                 ; move dx=3, dy=0, direction=left
                .db 6                   ; pen up
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 224                 ; move dx=3, dy=0, direction=rigt
                ;; ascii 62: '>'
                .db 2                   ; # moves
                .db 2                   ; origin x
                .db 2                   ; origin y
                .db 213                 ; move dx=2, dy=2, direction=right down
                .db 215                 ; move dx=2, dy=2, direction=left down
                ;; ascii 63: '?'
                .db 8                   ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 215                 ; move dx=2, dy=2, direction=left down
                .db 6                   ; pen up
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 64: '@'
                .db 18                  ; # moves
                .db 5                   ; origin x
                .db 3                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 65: 'A'
                .db 9                   ; # moves
                .db 4                   ; origin x
                .db 4                   ; origin y
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 164                 ; move dx=1, dy=0, direction=down
                ;; ascii 66: 'B'
                .db 12                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 230                 ; move dx=3, dy=0, direction=left
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                ;; ascii 67: 'C'
                .db 11                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 7                   ; pen down, point set
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 68: 'D'
                .db 7                   ; # moves
                .db 4                   ; origin x
                .db 5                   ; origin y
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 211                 ; move dx=2, dy=2, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 69: 'E'
                .db 9                   ; # moves
                .db 5                   ; origin x
                .db 1                   ; origin y
                .db 134                 ; move dx=0, dy=0, direction=left
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 70: 'F'
                .db 8                   ; # moves
                .db 5                   ; origin x
                .db 1                   ; origin y
                .db 134                 ; move dx=0, dy=0, direction=left
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 148                 ; move dx=0, dy=2, direction=down
                ;; ascii 71: 'G'
                .db 11                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 72: 'H'
                .db 11                  ; # moves
                .db 1                   ; origin x
                .db 6                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 6                   ; pen up
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 73: 'I'
                .db 9                   ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 74: 'J'
                .db 9                   ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 75: 'K'
                .db 8                   ; # moves
                .db 1                   ; origin x
                .db 6                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 6                   ; pen up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 7                   ; pen down, point set
                .db 215                 ; move dx=2, dy=2, direction=left down
                .db 253                 ; move dx=3, dy=3, direction=right down
                ;; ascii 76: 'L'
                .db 3                   ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 77: 'M'
                .db 6                   ; # moves
                .db 1                   ; origin x
                .db 6                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 213                 ; move dx=2, dy=2, direction=right down
                .db 209                 ; move dx=2, dy=2, direction=rigt up
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 78: 'N'
                .db 10                  ; # moves
                .db 5                   ; origin x
                .db 1                   ; origin y
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 163                 ; move dx=1, dy=0, direction=left up
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 6                   ; pen up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 7                   ; pen down, point set
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 79: 'O'
                .db 7                   ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 154                 ; move dx=0, dy=3, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 80: 'P'
                .db 10                  ; # moves
                .db 5                   ; origin x
                .db 3                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 230                 ; move dx=3, dy=0, direction=left
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 81: 'Q'
                .db 9                   ; # moves
                .db 3                   ; origin x
                .db 4                   ; origin y
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 154                 ; move dx=0, dy=3, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 82: 'R'
                .db 11                  ; # moves
                .db 5                   ; origin x
                .db 3                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 230                 ; move dx=3, dy=0, direction=left
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                ;; ascii 83: 'S'
                .db 13                  ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 84: 'T'
                .db 5                   ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 164                 ; move dx=1, dy=0, direction=down
                ;; ascii 85: 'U'
                .db 9                   ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 6                   ; pen up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 86: 'V'
                .db 9                   ; # moves
                .db 2                   ; origin x
                .db 5                   ; origin y
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 154                 ; move dx=0, dy=3, direction=up
                .db 6                   ; pen up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 7                   ; pen down, point set
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 215                 ; move dx=2, dy=2, direction=left down
                ;; ascii 87: 'W'
                .db 10                  ; # moves
                .db 3                   ; origin x
                .db 5                   ; origin y
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 6                   ; pen up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 7                   ; pen down, point set
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 88: 'X'
                .db 10                  ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 213                 ; move dx=2, dy=2, direction=right down
                .db 209                 ; move dx=2, dy=2, direction=rigt up
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 209                 ; move dx=2, dy=2, direction=rigt up
                .db 213                 ; move dx=2, dy=2, direction=right down
                ;; ascii 89: 'Y'
                .db 9                   ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 215                 ; move dx=2, dy=2, direction=left down
                .db 148                 ; move dx=0, dy=2, direction=down
                ;; ascii 90: 'Z'
                .db 4                   ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 167                 ; move dx=1, dy=0, direction=left down
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 91: '['
                .db 4                   ; # moves
                .db 5                   ; origin x
                .db 1                   ; origin y
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 92: '\'
                .db 1                   ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 165                 ; move dx=1, dy=0, direction=right down
                ;; ascii 93: ']'
                .db 6                   ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 94: '^'
                .db 8                   ; # moves
                .db 1                   ; origin x
                .db 3                   ; origin y
                .db 209                 ; move dx=2, dy=2, direction=rigt up
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 148                 ; move dx=0, dy=2, direction=down
                ;; ascii 95: '_'
                .db 2                   ; # moves
                .db 0                   ; origin x
                .db 7                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 160                 ; move dx=1, dy=0, direction=rigt
                ;; ascii 96: '`'
                .db 8                   ; # moves
                .db 5                   ; origin x
                .db 2                   ; origin y
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 215                 ; move dx=2, dy=2, direction=left down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 97: 'a'
                .db 8                   ; # moves
                .db 2                   ; origin x
                .db 2                   ; origin y
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                ;; ascii 98: 'b'
                .db 13                  ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 224                 ; move dx=3, dy=0, direction=rigt
                ;; ascii 99: 'c'
                .db 5                   ; # moves
                .db 5                   ; origin x
                .db 2                   ; origin y
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 100: 'd'
                .db 9                   ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                ;; ascii 101: 'e'
                .db 11                  ; # moves
                .db 5                   ; origin x
                .db 3                   ; origin y
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                ;; ascii 102: 'f'
                .db 5                   ; # moves
                .db 5                   ; origin x
                .db 1                   ; origin y
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 156                 ; move dx=0, dy=3, direction=down
                ;; ascii 103: 'g'
                .db 10                  ; # moves
                .db 5                   ; origin x
                .db 6                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 230                 ; move dx=3, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 104: 'h'
                .db 8                   ; # moves
                .db 1                   ; origin x
                .db 6                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 6                   ; pen up
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 148                 ; move dx=0, dy=2, direction=down
                ;; ascii 105: 'i'
                .db 8                   ; # moves
                .db 3                   ; origin x
                .db 1                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 106: 'j'
                .db 11                  ; # moves
                .db 5                   ; origin x
                .db 1                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 6                   ; pen up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                ;; ascii 107: 'k'
                .db 7                   ; # moves
                .db 2                   ; origin x
                .db 6                   ; origin y
                .db 146                 ; move dx=0, dy=2, direction=up
                .db 209                 ; move dx=2, dy=2, direction=rigt up
                .db 6                   ; pen up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 7                   ; pen down, point set
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 253                 ; move dx=3, dy=3, direction=right down
                ;; ascii 108: 'l'
                .db 3                   ; # moves
                .db 3                   ; origin x
                .db 1                   ; origin y
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                ;; ascii 109: 'm'
                .db 9                   ; # moves
                .db 2                   ; origin x
                .db 2                   ; origin y
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 6                   ; pen up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 7                   ; pen down, point set
                .db 154                 ; move dx=0, dy=3, direction=up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 156                 ; move dx=0, dy=3, direction=down
                ;; ascii 110: 'n'
                .db 4                   ; # moves
                .db 1                   ; origin x
                .db 6                   ; origin y
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 224                 ; move dx=3, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 156                 ; move dx=0, dy=3, direction=down
                ;; ascii 111: 'o'
                .db 7                   ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 146                 ; move dx=0, dy=2, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 112: 'p'
                .db 10                  ; # moves
                .db 5                   ; origin x
                .db 4                   ; origin y
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 230                 ; move dx=3, dy=0, direction=left
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 224                 ; move dx=3, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 113: 'q'
                .db 7                   ; # moves
                .db 4                   ; origin x
                .db 5                   ; origin y
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 171                 ; move dx=1, dy=1, direction=left up
                .db 138                 ; move dx=0, dy=1, direction=up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 224                 ; move dx=3, dy=0, direction=rigt
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 114: 'r'
                .db 3                   ; # moves
                .db 5                   ; origin x
                .db 2                   ; origin y
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 156                 ; move dx=0, dy=3, direction=down
                ;; ascii 115: 's'
                .db 11                  ; # moves
                .db 4                   ; origin x
                .db 2                   ; origin y
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 224                 ; move dx=3, dy=0, direction=rigt
                ;; ascii 116: 't'
                .db 6                   ; # moves
                .db 3                   ; origin x
                .db 1                   ; origin y
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                ;; ascii 117: 'u'
                .db 9                   ; # moves
                .db 5                   ; origin x
                .db 5                   ; origin y
                .db 154                 ; move dx=0, dy=3, direction=up
                .db 6                   ; pen up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 7                   ; pen down, point set
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 118: 'v'
                .db 11                  ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 119: 'w'
                .db 9                   ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 146                 ; move dx=0, dy=2, direction=up
                .db 6                   ; pen up
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 7                   ; pen down, point set
                .db 156                 ; move dx=0, dy=3, direction=down
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 120: 'x'
                .db 11                  ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 161                 ; move dx=1, dy=0, direction=rigt up
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 173                 ; move dx=1, dy=1, direction=right down
                ;; ascii 121: 'y'
                .db 12                  ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 162                 ; move dx=1, dy=0, direction=up
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 122: 'z'
                .db 3                   ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 128                 ; move dx=0, dy=0, direction=rigt
                .db 167                 ; move dx=1, dy=0, direction=left down
                .db 128                 ; move dx=0, dy=0, direction=rigt
                ;; ascii 123: '{'
                .db 9                   ; # moves
                .db 5                   ; origin x
                .db 1                   ; origin y
                .db 198                 ; move dx=2, dy=0, direction=left
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 124: '|'
                .db 2                   ; # moves
                .db 3                   ; origin x
                .db 1                   ; origin y
                .db 164                 ; move dx=1, dy=0, direction=down
                .db 140                 ; move dx=0, dy=1, direction=down
                ;; ascii 125: '}'
                .db 12                  ; # moves
                .db 1                   ; origin x
                .db 1                   ; origin y
                .db 192                 ; move dx=2, dy=0, direction=rigt
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 140                 ; move dx=0, dy=1, direction=down
                .db 6                   ; pen up
                .db 175                 ; move dx=1, dy=1, direction=left down
                .db 7                   ; pen down, point set
                .db 192                 ; move dx=2, dy=0, direction=rigt
                ;; ascii 126: '~'
                .db 5                   ; # moves
                .db 1                   ; origin x
                .db 2                   ; origin y
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 6                   ; pen up
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 7                   ; pen down, point set
                .db 175                 ; move dx=1, dy=1, direction=left down
                ;; ascii 127: '<non standard>'
                .db 12                  ; # moves
                .db 3                   ; origin x
                .db 3                   ; origin y
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 160                 ; move dx=1, dy=0, direction=rigt
                .db 173                 ; move dx=1, dy=1, direction=right down
                .db 169                 ; move dx=1, dy=1, direction=rigt up
                .db 146                 ; move dx=0, dy=2, direction=up
                .db 211                 ; move dx=2, dy=2, direction=left up
                .db 166                 ; move dx=1, dy=0, direction=left
                .db 215                 ; move dx=2, dy=2, direction=left down
                .db 148                 ; move dx=0, dy=2, direction=down
                .db 213                 ; move dx=2, dy=2, direction=right down
                .db 160                 ; move dx=1, dy=0, direction=rigt
