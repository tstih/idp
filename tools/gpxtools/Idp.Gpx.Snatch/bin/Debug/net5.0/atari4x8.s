                ;;      atari4x8.s
                ;;      atari4x8 font
                ;;
                ;;      notes: see font.h for format details
                .module atari4x8_font

                .globl atari4x8_font

                .area _CODE
atari4x8_font::
                ;; font header
                .db 17                  ;; font generation
                .db 4                   ;; font (max) width
                .db 8                   ;; font height
                .db 1                   ;; width in bytes
                .db 0                   ;; first ascii
                .db 126                 ;; last ascii

                ;; ascii 0: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 1: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 2: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 3: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 4: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 5: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 6: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 7: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 8: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 9: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 10: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 11: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 12: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 13: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 14: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 15: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 16: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 17: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 18: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 19: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 20: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 21: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 22: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 23: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 24: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 25: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 26: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 27: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 28: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 29: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 30: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 31: '<control>'
                
                
                
                
                
                
                
                

                ;; ascii 32: ' '
                
                
                
                
                
                
                
                

                ;; ascii 33: '!'
                
                
                
                
                
                
                
                

                ;; ascii 34: '"'
                
                
                
                
                
                
                
                

                ;; ascii 35: '#'
                
                
                
                
                
                
                
                

                ;; ascii 36: '$'
                
                
                
                
                
                
                
                

                ;; ascii 37: '%'
                
                
                
                
                
                
                
                

                ;; ascii 38: '&'
                
                
                
                
                
                
                
                

                ;; ascii 39: '''
                
                
                
                
                
                
                
                

                ;; ascii 40: '('
                
                
                
                
                
                
                
                

                ;; ascii 41: ')'
                
                
                
                
                
                
                
                

                ;; ascii 42: '*'
                
                
                
                
                
                
                
                

                ;; ascii 43: '+'
                
                
                
                
                
                
                
                

                ;; ascii 44: ','
                
                
                
                
                
                
                
                

                ;; ascii 45: '-'
                
                
                
                
                
                
                
                

                ;; ascii 46: '.'
                
                
                
                
                
                
                
                

                ;; ascii 47: '/'
                
                
                
                
                
                
                
                

                ;; ascii 48: '0'
                
                
                
                
                
                
                
                

                ;; ascii 49: '1'
                
                
                
                
                
                
                
                

                ;; ascii 50: '2'
                
                
                
                
                
                
                
                

                ;; ascii 51: '3'
                
                
                
                
                
                
                
                

                ;; ascii 52: '4'
                
                
                
                
                
                
                
                

                ;; ascii 53: '5'
                
                
                
                
                
                
                
                

                ;; ascii 54: '6'
                
                
                
                
                
                
                
                

                ;; ascii 55: '7'
                
                
                
                
                
                
                
                

                ;; ascii 56: '8'
                
                
                
                
                
                
                
                

                ;; ascii 57: '9'
                
                
                
                
                
                
                
                

                ;; ascii 58: ':'
                
                
                
                
                
                
                
                

                ;; ascii 59: ';'
                
                
                
                
                
                
                
                

                ;; ascii 60: '<'
                
                
                
                
                
                
                
                

                ;; ascii 61: '='
                
                
                
                
                
                
                
                

                ;; ascii 62: '>'
                
                
                
                
                
                
                
                

                ;; ascii 63: '?'
                
                
                
                
                
                
                
                

                ;; ascii 64: '@'
                
                
                
                
                
                
                
                

                ;; ascii 65: 'A'
                
                
                
                
                
                
                
                

                ;; ascii 66: 'B'
                
                
                
                
                
                
                
                

                ;; ascii 67: 'C'
                
                
                
                
                
                
                
                

                ;; ascii 68: 'D'
                
                
                
                
                
                
                
                

                ;; ascii 69: 'E'
                
                
                
                
                
                
                
                

                ;; ascii 70: 'F'
                
                
                
                
                
                
                
                

                ;; ascii 71: 'G'
                
                
                
                
                
                
                
                

                ;; ascii 72: 'H'
                
                
                
                
                
                
                
                

                ;; ascii 73: 'I'
                
                
                
                
                
                
                
                

                ;; ascii 74: 'J'
                
                
                
                
                
                
                
                

                ;; ascii 75: 'K'
                
                
                
                
                
                
                
                

                ;; ascii 76: 'L'
                
                
                
                
                
                
                
                

                ;; ascii 77: 'M'
                
                
                
                
                
                
                
                

                ;; ascii 78: 'N'
                
                
                
                
                
                
                
                

                ;; ascii 79: 'O'
                
                
                
                
                
                
                
                

                ;; ascii 80: 'P'
                
                
                
                
                
                
                
                

                ;; ascii 81: 'Q'
                
                
                
                
                
                
                
                

                ;; ascii 82: 'R'
                
                
                
                
                
                
                
                

                ;; ascii 83: 'S'
                
                
                
                
                
                
                
                

                ;; ascii 84: 'T'
                
                
                
                
                
                
                
                

                ;; ascii 85: 'U'
                
                
                
                
                
                
                
                

                ;; ascii 86: 'V'
                
                
                
                
                
                
                
                

                ;; ascii 87: 'W'
                
                
                
                
                
                
                
                

                ;; ascii 88: 'X'
                
                
                
                
                
                
                
                

                ;; ascii 89: 'Y'
                
                
                
                
                
                
                
                

                ;; ascii 90: 'Z'
                
                
                
                
                
                
                
                

                ;; ascii 91: '['
                
                
                
                
                
                
                
                

                ;; ascii 92: '\'
                
                
                
                
                
                
                
                

                ;; ascii 93: ']'
                
                
                
                
                
                
                
                

                ;; ascii 94: '^'
                
                
                
                
                
                
                
                

                ;; ascii 95: '_'
                
                
                
                
                
                
                
                

                ;; ascii 96: '`'
                
                
                
                
                
                
                
                

                ;; ascii 97: 'a'
                
                
                
                
                
                
                
                

                ;; ascii 98: 'b'
                
                
                
                
                
                
                
                

                ;; ascii 99: 'c'
                
                
                
                
                
                
                
                

                ;; ascii 100: 'd'
                
                
                
                
                
                
                
                

                ;; ascii 101: 'e'
                
                
                
                
                
                
                
                

                ;; ascii 102: 'f'
                
                
                
                
                
                
                
                

                ;; ascii 103: 'g'
                
                
                
                
                
                
                
                

                ;; ascii 104: 'h'
                
                
                
                
                
                
                
                

                ;; ascii 105: 'i'
                
                
                
                
                
                
                
                

                ;; ascii 106: 'j'
                
                
                
                
                
                
                
                

                ;; ascii 107: 'k'
                
                
                
                
                
                
                
                

                ;; ascii 108: 'l'
                
                
                
                
                
                
                
                

                ;; ascii 109: 'm'
                
                
                
                
                
                
                
                

                ;; ascii 110: 'n'
                
                
                
                
                
                
                
                

                ;; ascii 111: 'o'
                
                
                
                
                
                
                
                

                ;; ascii 112: 'p'
                
                
                
                
                
                
                
                

                ;; ascii 113: 'q'
                
                
                
                
                
                
                
                

                ;; ascii 114: 'r'
                
                
                
                
                
                
                
                

                ;; ascii 115: 's'
                
                
                
                
                
                
                
                

                ;; ascii 116: 't'
                
                
                
                
                
                
                
                

                ;; ascii 117: 'u'
                
                
                
                
                
                
                
                

                ;; ascii 118: 'v'
                
                
                
                
                
                
                
                

                ;; ascii 119: 'w'
                
                
                
                
                
                
                
                

                ;; ascii 120: 'x'
                
                
                
                
                
                
                
                

                ;; ascii 121: 'y'
                
                
                
                
                
                
                
                

                ;; ascii 122: 'z'
                
                
                
                
                
                
                
                

                ;; ascii 123: '{'
                
                
                
                
                
                
                
                

                ;; ascii 124: '|'
                
                
                
                
                
                
                
                

                ;; ascii 125: '}'
                
                
                
                
                
                
                
                

                ;; ascii 126: '~'
                
                
                
                
                
                
                
                

