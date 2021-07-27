#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
/*Some useful Macros ,Definitions*/
#define INF 32000
#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))
/*
typedef enum
{
    FALSE, TRUE
} BOOL;
*/
/*Phase of Game*/
typedef enum
{
    END, MID, OPEN
} STATE;
/*Board Representation*/
typedef enum
{
    A1, A2, A3, A4, A5, A6, A7, A8,
    B1, B2, B3, B4, B5, B6, B7, B8,
    C1, C2, C3, C4, C5, C6, C7, C8,
    D1, D2, D3, D4, D5, D6, D7, D8,
    E1, E2, E3, E4, E5, E6, E7, E8,
    F1, F2, F3, F4, F5, F6, F7, F8,
    G1, G2, G3, G4, G5, G6, G7, G8,
    H1, H2, H3, H4, H5, H6, H7, H8,
    CASTLE, EP, LAST
} squares;
/*Pieces*/
typedef enum
{
    EMPTY,
    WHITE_KING, WHITE_QUEEN, WHITE_ROOK,
    WHITE_BISHOP, WHITE_KNIGHT, WHITE_PAWN,
    BLACK_KING, BLACK_QUEEN, BLACK_ROOK,
    BLACK_BISHOP, BLACK_KNIGHT, BLACK_PAWN
} pieces;
typedef unsigned char byte;
int pawns_white = 0;
int pawns_black = 0;
int bishops_white = 0;
int bishops_black = 0;
int rooks_white = 0;
int rooks_black = 0;
int queen_white = 0;
int all_pieces = 0;
int queen_black = 0;
int knights_white = 0;
int knights_black = 0;
/*Number of Half moves done so far*/
int ply;
/*Whose turn To Move*/
#define WTM (~ply & 1)
/*Look in Move generation for more about this Castle*/
byte castle[64];
#define CASTLE_WHITE_KING  1
#define CASTLE_WHITE_QUEEN 2
#define CASTLE_BLACK_KING  4
#define CASTLE_BLACK_QUEEN 8
/*Initial State of Board */
byte board[67] =
{
    WHITE_ROOK,   WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_PAWN, BLACK_ROOK,
    WHITE_KNIGHT, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_PAWN, BLACK_KNIGHT,
    WHITE_BISHOP, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_PAWN, BLACK_BISHOP,
    WHITE_QUEEN,  WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_PAWN, BLACK_QUEEN,
    WHITE_KING,   WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_PAWN, BLACK_KING,
    WHITE_BISHOP, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_PAWN, BLACK_BISHOP,
    WHITE_KNIGHT, WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_PAWN, BLACK_KNIGHT,
    WHITE_ROOK,   WHITE_PAWN, EMPTY, EMPTY, EMPTY, EMPTY, BLACK_PAWN, BLACK_ROOK,
    CASTLE_BLACK_QUEEN | CASTLE_BLACK_KING | CASTLE_WHITE_QUEEN | CASTLE_WHITE_KING,
    EMPTY, 0,
};
/*For more about this look in atk_slide function*/
const byte king_dirs[64] =
{
    7,  31,  31,  31,  31,  31,  31,  28,
    199, 255, 255, 255, 255, 255, 255, 124,
    199, 255, 255, 255, 255, 255, 255, 124,
    199, 255, 255, 255, 255, 255, 255, 124,
    199, 255, 255, 255, 255, 255, 255, 124,
    199, 255, 255, 255, 255, 255, 255, 124,
    199, 255, 255, 255, 255, 255, 255, 124,
    193, 241, 241, 241, 241, 241, 241, 112,
};
const byte knight_dirs[64] =
{
    6,  14,  46,  46,  46,  46,  44,  40,
    7,  15,  63,  63,  63,  63,  60,  56,
    71, 207, 255, 255, 255, 255, 252, 184,
    71, 207, 255, 255, 255, 255, 252, 184,
    71, 207, 255, 255, 255, 255, 252, 184,
    71, 207, 255, 255, 255, 255, 252, 184,
    67, 195, 243, 243, 243, 243, 240, 176,
    65, 193, 209, 209, 209, 209, 208, 144,
};
int phase;
int pawn_value[64] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -5, -5, 0, 0, 0,
    1, 2, 3, 4, 4, 3, 2, 1,
    2, 4, 6, 8, 8, 6, 4, 2,
    3, 6, 9, 12, 12, 9, 6, 3,
    4, 8, 12, 16, 16, 12, 8, 4,
    5, 10, 15, 20, 20, 15, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0,
};
int bishop_value[64] =
{
    -5, -5, -5, -5, -5, -5, -5, -5,
    -5, 10, 5, 10, 10, 5, 10, -5,
    -5, 5, 3, 12, 12, 3, 5, -5,
    -5, 3, 12, 3, 3, 12, 3, -5,
    -5, 3, 12, 3, 3, 12, 3, -5,
    -5, 5, 3, 12, 12, 3, 5, -5,
    -5, 10, 5, 10, 10, 5, 10, -5,
    -5, -5, -5, -5, -5, -5, -5, -5,
};
int knight_value[64] =
{
    -10, -5, -5, -5, -5, -5, -5, -10,
    -5, 0, 0, 3, 3, 0, 0, -5,
    -5, 0, 5, 5, 5, 5, 0, -5,
    -5, 0, 5, 10, 10, 5, 0, -5,
    -5, 0, 5, 10, 10, 5, 0, -5,
    -5, 0, 5, 5, 5, 5, 0, -5,
    -5, 0, 0, 3, 3, 0, 0, -5,
    -10, -5, -5, -5, -5, -5, -5, -10,
};
int king_value_open[64] =
{
    2, 10, 4, 0, 0, 7, 10, 2,
    -3, -3, -5, -5, -5, -5, -3, -3,
    -5, -5, -8, -8, -8, -8, -5, -5,
    -8, -8, -13, -13, -13, -13, -8, -8,
    -13, -13, -21, -21, -21, -21, -13, -13,
    -21, -21, -34, -34, -34, -34, -21, -21,
    -34, -34, -55, -55, -55, -55, -34, -34,
    -55, -55, -89, -89, -89, -89, -55, -55,
};
int king_value_end[64] =
{
    -5, -3, -1, 0, 0, -1, -3, -5,
    -3, 5, 5, 5, 5, 5, 5, -3,
    -1, 5, 10, 10, 10, 10, 5, -1,
    0, 5, 10, 15, 15, 10, 5, 0,
    0, 5, 10, 15, 15, 10, 5, 0,
    -1, 5, 10, 10, 10, 10, 5, -1,
    -3, 5, 5, 5, 5, 5, 5, -3,
    -5, -3, -1, 0, 0, -1, -3, -5,
};
/*Which Sides Does computer control*/
int computer[2];
typedef struct _side
{
    byte attack[64];
    int king;
    byte pawns[10];
} side;
side white, black, *friend, *enemy;
signed char undo_stack[6 * 1024], *undo_sp;
int maxdepth = 4;
/*Look in Search Functions*/
#define PRESCORE_EQUAL       100
const int prescore_piece_value[] =
{
    0,
    0, 9, 5, 3, 3, 1 ,
    0, 9, 5, 3, 3, 1 ,
};
/*Move structure*/
typedef struct _s_move
{
    short move;
    unsigned short prescore;
} s_move;
s_move move_stack[1024], *move_sp;
#define PIECE_COLOR(pc) ((pc) < BLACK_KING)
#define DIR_N (+1)
#define DIR_E (+8)
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
#define RANK2CHAR(r)            ('1'+(r))
#define CHAR2RANK(c)            ((c)-'1')
#define FILE2CHAR(f)            ('a'+(f))
#define CHAR2FILE(c)            ((c)-'a')
#define PIECE2CHAR(p)           (" KQRBNPkqrbnp"[p])
#define F(square)               ((square)/(8))
#define R(square)               ((square)%(8))
#define SQ(f,r)                 ((f)*(8) + (r))
#define FLIP(square)            ((square)^7)
#define MOVE(fr,to)             (((fr) *(64)) + (to))
#define FR(move)                (((move) & 07700) >> 6)
#define TO(move)                ((move) & 00077)
#define SPECIAL                 (1<<12)
typedef struct _command
{
    char *name;
    void (*cmd)(char *);
} command;
#define ATK_NORTH               0b00000001
#define ATK_NORTHEAST           0b00000010
#define ATK_EAST                0b00000100
#define ATK_SOUTHEAST           0b00001000
#define ATK_SOUTH               0b00010000
#define ATK_SOUTHWEST           0b00100000
#define ATK_WEST                0b01000000
#define ATK_NORTHWEST           0b10000000
#define ATK_ORTHOGONAL          ( ATK_NORTH | ATK_SOUTH |ATK_WEST  | ATK_EAST  )
#define ATK_DIAGONAL            ( ATK_NORTHEAST | ATK_NORTHWEST |ATK_SOUTHEAST | ATK_SOUTHWEST )
#define ATK_SLIDER              ( ATK_ORTHOGONAL | ATK_DIAGONAL )
#define base                    0b00000001
signed char king_step[129];
signed char knight_jump[129];
void print_square(int square)
{
    putchar(FILE2CHAR(F(square)));
    putchar(RANK2CHAR(R(square)));
}
void print_board()
{
    int file, rank;
    for (rank = RANK_8; rank >= RANK_1; rank--)
    {
        printf("%d ", 1 + rank);
        for (file = FILE_A; file <= FILE_H; file++)
            printf(" %c |", PIECE2CHAR(board[SQ(file, rank)]));
        printf("\n  ---+---+---+---+---+---+---+---+\n");
    }
    printf(" | a | b | c | d | e | f | g | h |\n\n");
}
void reset()
{
    move_sp = move_stack;
    undo_sp = undo_stack;
}
void atk_slide(int sq, byte dirs, side *s)
{
    int to, i;
    byte dir = 0;
    /*dirs contains the directions in which we can move
    piece at the given square given no hurdles*/
    dirs &= king_dirs[sq];
    for (i = 0; i < 8; i++)
    {
        /*dir contains the present direction
        we are looking into*/
        dir = base << i;
        to = sq;
        /*if piece can move in that direction ....*/
        if ( (base << i) & dirs )
        {
            /*then ...continue looking in that direction
            until you find hurdle or fall out of board*/
            while (TRUE)
            {
                to += king_step[dir];
                s->attack[to] += 1;
                if (board[to] != EMPTY || !(dir & king_dirs[to]))
                    break;
            }
        }
    }
}
/*compute attacks finds squares with
number of attacks of whites or blacks on it*/
void compute_attacks()
{
    int sq, to, pc;
    byte dir, dirs;
    memset(&white, 0, sizeof (white));
    memset(&black, 0, sizeof (black));
    friend = WTM ? &white : &black;
    enemy = WTM ? &black : &white;
    for (sq = 0; sq < 64; sq++)
    {
        pc = board[sq];
        if (pc == EMPTY) continue;
        switch (pc)
        {
        case WHITE_KING:
            dir = 0;
            white.king = sq;
            dirs = king_dirs[sq];
            while (TRUE)
            {
                dir -= dirs;
                dir &= dirs;
                to = sq + king_step[dir];
                white.attack[to] += 1;
                if (!(dirs -= dir)) break;
            }
            break;
        case BLACK_KING:
            dir = 0;
            black.king = sq;
            dirs = king_dirs[sq];
            while (TRUE)
            {
                dir -= dirs;
                dir &= dirs;
                to = sq + king_step[dir];
                black.attack[to] += 1;
                if (!(dirs -= dir)) break;
            }
            break;
        case WHITE_QUEEN:
            atk_slide(sq, ATK_SLIDER, &white);
            break;
        case BLACK_QUEEN:
            atk_slide(sq, ATK_SLIDER, &black);
            break;
        case WHITE_ROOK:
            atk_slide(sq, ATK_ORTHOGONAL, &white);
            break;
        case BLACK_ROOK:
            atk_slide(sq, ATK_ORTHOGONAL, &black);
            break;
        case WHITE_BISHOP:
            atk_slide(sq, ATK_DIAGONAL, &white);
            break;
        case BLACK_BISHOP:
            atk_slide(sq, ATK_DIAGONAL, &black);
            break;
        case WHITE_KNIGHT:
            dir = 0;
            dirs = knight_dirs[sq];
            while (TRUE)
            {
                dir -= dirs;
                dir &= dirs;
                to = sq + knight_jump[dir];
                white.attack[to] += 1;
                if (!(dirs -= dir))
                    break;
            }
            break;
        case BLACK_KNIGHT:
            dir = 0;
            dirs = knight_dirs[sq];
            while (TRUE)
            {
                dir -= dirs;
                dir &= dirs;
                to = sq + knight_jump[dir];
                black.attack[to] += 1;
                if (!(dirs -= dir))
                    break;
            }
            break;
        case WHITE_PAWN:
            white.pawns[1 + F(sq)] += 1;
            if (F(sq) != FILE_H)
                white.attack[sq + DIR_N + DIR_E] += 1;
            if (F(sq) != FILE_A)
                white.attack[sq + DIR_N - DIR_E] += 1;
            break;
        case BLACK_PAWN:
            black.pawns[1 + F(sq)] += 1;
            if (F(sq) != FILE_H)
                black.attack[sq - DIR_N + DIR_E] += 1;
            if (F(sq) != FILE_A)
                black.attack[sq - DIR_N - DIR_E] += 1;
            break;
        }
    }
}
void unmake_move()
{
    int sq;
    while (TRUE)
    {
        sq = *--undo_sp;
        if (sq < 0) break;
        board[sq] = *--undo_sp;
    }
    ply--;
}
void make_move(int move)
{
    int fr;
    int to;
    int sq;
    *undo_sp++ = -1;
    if (board[EP])
    {
        *undo_sp++ = board[EP];
        *undo_sp++ = EP;
        board[EP] = 0;
    }
    to = TO(move);
    fr = FR(move);
    if (move & SPECIAL)
    {
        switch (R(fr))
        {
        case RANK_8:
            unmake_move();
            if (to == G8)
                make_move(MOVE(H8, F8));
            else
                make_move(MOVE(A8, D8));
            break;
        case RANK_7:
            if (board[fr] == BLACK_PAWN)
            {
                *undo_sp++ = 0;
                *undo_sp++ = EP;
                board[EP] = to;
            }
            else
            {
                *undo_sp++ = board[fr];
                *undo_sp++ = fr;
                board[fr] = WHITE_QUEEN + (move >> 13);
            }
            break;
        case RANK_5:
        case RANK_4:
            sq = SQ(F(to), R(fr));
            *undo_sp++ = board[sq];
            *undo_sp++ = sq;
            board[sq] = EMPTY;
            break;
        case RANK_2:
            if (board[fr] == WHITE_PAWN)
            {
                *undo_sp++ = 0;
                *undo_sp++ = EP;
                board[EP] = to;
            }
            else
            {
                *undo_sp++ = board[fr];
                *undo_sp++ = fr;
                board[fr] = BLACK_QUEEN + (move >> 13);
            }
            break;
        case RANK_1:
            unmake_move();
            if (to == G1)
                make_move(MOVE(H1, F1));
            else
                make_move(MOVE(A1, D1));
            break;
        default:
            break;
        }
    }
    ply++;
    if (board[to] != EMPTY || board[fr] == WHITE_PAWN || board[fr] == BLACK_PAWN)
    {
        *undo_sp++ = board[LAST];
        *undo_sp++ = LAST;
        board[LAST] = ply;
    }
    *undo_sp++ = board[to];
    *undo_sp++ = to;
    *undo_sp++ = board[fr];
    *undo_sp++ = fr;
    board[to] = board[fr];
    board[fr] = EMPTY;
    if (board[CASTLE] & (castle[fr] | castle[to]))
    {
        *undo_sp++ = board[CASTLE];
        *undo_sp++ = CASTLE;
        board[CASTLE] &= ~(castle[fr] | castle[to]);
    }
}
unsigned short caps;
int push_move(int fr, int to)
{
    unsigned short prescore = PRESCORE_EQUAL;
    int move;
    if (board[to] != EMPTY)
        prescore += (1 << 9) + prescore_piece_value[board[to]];
    if (WTM)
    {
        if (black.attack[to] != 0)
            prescore -= prescore_piece_value[board[fr]];
        else if (white.attack[to] != 0)
            prescore -= prescore_piece_value[board[fr]];
    }
    if (prescore >= caps)
    {
        move = MOVE(fr, to);
        move_sp->move = move;
        move_sp->prescore = prescore;
        move_sp++;
        return 1;
    }
    return 0;
}
void push_special_move(int fr, int to)
{
    int move;
    move = MOVE(fr, to);
    move_sp->prescore = PRESCORE_EQUAL;
    move_sp->move = move | SPECIAL;
    move_sp++;
}
void push_pawn_move(int fr, int to)
{
    if ((R(to) == RANK_8) || (R(to) == RANK_1))
    {
        push_special_move(fr, to);
        push_special_move(fr, to);
        move_sp[-1].move += 1 << 13;
        push_special_move(fr, to);
        move_sp[-1].move += 2 << 13;
        push_special_move(fr, to);
        move_sp[-1].move += 3 << 13;
    }
    else
        push_move(fr, to);
}
/*genarate sliding moves*/
void gen_slides(int fr, byte dirs)
{
    int to, i;
    byte dir = 0;
    dirs &= king_dirs[fr];
    for (i = 0; i < 8; i++)
    {
        dir = base << i;
        to = fr;
        if (dir & dirs)
        {
            while (TRUE)
            {
                to += king_step[dir];
                if (board[to] != EMPTY)
                {
                    if (PIECE_COLOR(board[to]) != WTM)
                        push_move(fr, to);
                    break;
                }
                push_move(fr, to);
                if (!(dir & king_dirs[to]))
                    break;
            }
        }
    }
}
/*compare two moves based on their Prescore
When we quick sort the moves we will end up
searching good moves first*/
int cmp_move(const void *ap, const void *bp)
{
    const s_move *a = ap;
    const s_move *b = bp;
    if (a->prescore < b->prescore) return -1;
    if (a->prescore > b->prescore) return 1;
    return a->move - b->move;
}
void generate_moves(unsigned treshold)
{
    int             fr, to;
    int             pc;
    byte            dir, dirs;
    caps = treshold;
    for (fr = 0; fr < 64; fr++)
    {
        pc = board[fr];
        if (!pc || PIECE_COLOR(pc) != WTM) continue;
        switch (pc)
        {
        case WHITE_KING:
        case BLACK_KING:
            dir = 0;
            dirs = king_dirs[fr];
            do
            {
                dir -= dirs;
                dir &= dirs;
                to = fr + king_step[dir];
                if (board[to] != EMPTY &&
                        PIECE_COLOR(board[to]) == WTM) continue;
                push_move(fr, to);
            }
            while (dirs -= dir);
            break;
        case WHITE_QUEEN:
        case BLACK_QUEEN:
            gen_slides(fr, ATK_SLIDER);
            break;
        case WHITE_ROOK:
        case BLACK_ROOK:
            gen_slides(fr, ATK_ORTHOGONAL);
            break;
        case WHITE_BISHOP:
        case BLACK_BISHOP:
            gen_slides(fr, ATK_DIAGONAL);
            break;
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            dir = 0;
            dirs = knight_dirs[fr];
            do
            {
                dir -= dirs;
                dir &= dirs;
                to = fr + knight_jump[dir];
                if (board[to] != EMPTY &&
                        PIECE_COLOR(board[to]) == WTM) continue;
                push_move(fr, to);
            }
            while (dirs -= dir);
            break;
        case WHITE_PAWN:
            if (F(fr) != FILE_H)
            {
                to = fr + DIR_N + DIR_E;
                if (board[to] >= BLACK_KING)
                    push_pawn_move(fr, to);
            }
            if (F(fr) != FILE_A)
            {
                to = fr + DIR_N - DIR_E;
                if (board[to] >= BLACK_KING)
                    push_pawn_move(fr, to);
            }
            to = fr + DIR_N;
            if (board[to] != EMPTY)
                break;
            push_pawn_move(fr, to);
            if (R(fr) == RANK_2)
            {
                to += DIR_N;
                if (board[to] == EMPTY)
                    if (push_move(fr, to))
                        if (black.attack[to - DIR_N])
                            move_sp[-1].move |= SPECIAL;
            }
            break;
        case BLACK_PAWN:
            if (F(fr) != FILE_H)
            {
                to = fr - DIR_N + DIR_E;
                if (board[to] && board[to] < BLACK_KING)
                    push_pawn_move(fr, to);
            }
            if (F(fr) != FILE_A)
            {
                to = fr - DIR_N - DIR_E;
                if (board[to] && board[to] < BLACK_KING)
                    push_pawn_move(fr, to);
            }
            to = fr - DIR_N;
            if (board[to] != EMPTY)
                break;
            push_pawn_move(fr, to);
            if (R(fr) == RANK_7)
            {
                to -= DIR_N;
                if (board[to] == EMPTY)
                    if (push_move(fr, to))
                        if (white.attack[to + DIR_N])
                            move_sp[-1].move |= SPECIAL;

            }
            break;
        }
    }
    if (board[CASTLE] && !enemy->attack[friend->king])
    {
        if (WTM && (board[CASTLE] & CASTLE_WHITE_KING) &&
                !board[F1] && !board[G1] &&
                !enemy->attack[F1])
            push_special_move(E1, G1);
        if (WTM && (board[CASTLE] & CASTLE_WHITE_QUEEN) &&
                !board[D1] && !board[C1] && !board[B1] &&
                !enemy->attack[D1])
            push_special_move(E1, C1);
        if (!WTM && (board[CASTLE] & CASTLE_BLACK_KING) &&
                !board[F8] && !board[G8] &&
                !enemy->attack[F8])
            push_special_move(E8, G8);
        if (!WTM && (board[CASTLE] & CASTLE_BLACK_QUEEN) &&
                !board[D8] && !board[C8] && !board[B8] &&
                !enemy->attack[D8])
            push_special_move(E8, C8);
    }
    if (board[EP])
    {
        int ep = board[EP];

        if (WTM)
        {
            if (F(ep) != FILE_A && board[ep - DIR_E] == WHITE_PAWN)
                if (push_move(ep - DIR_E, ep + DIR_N))
                    move_sp[-1].move |= SPECIAL;
            if (F(ep) != FILE_H && board[ep + DIR_E] == WHITE_PAWN)
                if (push_move(ep + DIR_E, ep + DIR_N))
                    move_sp[-1].move |= SPECIAL;
        }
        else
        {
            if (F(ep) != FILE_A && board[ep - DIR_E] == BLACK_PAWN)
                if (push_move(ep - DIR_E, ep - DIR_N))
                    move_sp[-1].move |= SPECIAL;
            if (F(ep) != FILE_H && board[ep + DIR_E] == BLACK_PAWN)
                if (push_move(ep + DIR_E, ep - DIR_N))
                    move_sp[-1].move |= SPECIAL;
        }
    }
}
void print_move_san(int move)
{
    int fr, to;
    fr = FR(move);
    to = TO(move);
    printf("%c%c %c%c\n", FILE2CHAR(F(fr)), RANK2CHAR(R(fr)), FILE2CHAR(F(to)), RANK2CHAR(R(to)));
}
int test_is_check(int move)
{
    make_move(move);
    compute_attacks();
    unmake_move();
    return friend->attack[enemy->king] != 0;
}
/*Parse Move From Coordinate Notation*/
int parse_move(char *line)
{
    int move = 0;
    if (!strcmp(line, "o-o-o"))
        line = "e1c1";
    else if (!strcmp(line, "o-o"))
        line = "e1g1";
    s_move *m;
    m = move_sp;
    compute_attacks();
    generate_moves(0);
    int fr = SQ( CHAR2FILE(line[0]), CHAR2RANK(line[1]));
    int to = SQ( CHAR2FILE(line[2]), CHAR2RANK(line[3]));
    while (move_sp > m)
    {
        move_sp--;
        int sfr, sto;
        sfr = FR(move_sp->move);
        sto = TO(move_sp->move);
        if (fr != sfr || to != sto)
            continue;
        if (test_is_check(move_sp->move))
            continue;
        move = move_sp->move;
    }
    return move;
}
/*Under Construction*/
void number_of_pieces()
{
    pawns_white = 0;
    pawns_black = 0;
    bishops_white = 0;
    bishops_black = 0;
    rooks_white = 0;
    rooks_black = 0;
    queen_white = 0;
    all_pieces = 0;
    queen_black = 0;
    knights_white = 0;
    knights_black = 0;
    int i, piece;
    for (i = 0; i < 64; i++)
    {
        piece = board[i];
        if (piece == EMPTY)
            continue;
        switch (piece)
        {
        case WHITE_PAWN:
            all_pieces++;
            pawns_white++;
            break;
        case WHITE_QUEEN:
            all_pieces++;
            queen_white++;
            break;
        case WHITE_BISHOP:
            all_pieces++;
            bishops_white++;
            break;
        case WHITE_KNIGHT:
            all_pieces++;
            knights_white++;
            break;
        case WHITE_ROOK:
            all_pieces++;
            rooks_white++;
            break;
        case BLACK_PAWN:
            all_pieces++;
            pawns_white++;
            break;
        case BLACK_QUEEN:
            all_pieces++;
            queen_black++;
            break;
        case BLACK_BISHOP:
            all_pieces++;
            bishops_black++;
            break;
        case BLACK_KNIGHT:
            all_pieces++;
            knights_black++;
            break;
        case BLACK_ROOK:
            all_pieces++;
            rooks_black++;
            break;
        case WHITE_KING:
            all_pieces++;
            break;
        case BLACK_KING:
            all_pieces++;
            break;
        default:
            break;
        }
    }
}
int evaluate_phase()
{
    number_of_pieces();
    if (all_pieces <= 16)
        return END;
    else if (!(board[CASTLE] & CASTLE_BLACK_QUEEN) || !(board[CASTLE] & CASTLE_BLACK_KING) ||
             !(board[CASTLE] & CASTLE_WHITE_QUEEN) || (!board[CASTLE] & CASTLE_WHITE_KING))
        return MID;
    else return OPEN;
}
int evaluate()
{
    int sq, piece;
    int score = 0;
    float table_scores = 0;
    int white_has, black_has;
    white_has = 0;
    black_has = 0;
    for (sq = 0; sq < 64; sq++)
    {
        piece = board[sq];
        if (!piece) continue;
        switch (piece)
        {
        case WHITE_KING:
            if (sq == 6)
                white_has += 50;
            break;
        case BLACK_KING:
            if (sq == 62)
                black_has += -50;
            break;
        case WHITE_QUEEN:
            white_has += 200;
            break;
        case BLACK_QUEEN:
            black_has += 200;
            break;
        case WHITE_ROOK:
            white_has += 120;
            break;
        case BLACK_ROOK:
            black_has += 120;
            break;
        case WHITE_BISHOP:
            white_has += 70;
            break;
        case BLACK_BISHOP:
            black_has += 70;
            break;
        case WHITE_KNIGHT:
            white_has += 60;
            break;
        case BLACK_KNIGHT:
            black_has += 60;
            break;
        case WHITE_PAWN:
            white_has += 20;
            break;
        case BLACK_PAWN:
            black_has += 20;
            break;
        }
    }
    for (sq = 0; sq < 64; sq++)
    {
        white_has += white.attack[sq];
        black_has += black.attack[sq];
    }
    phase = evaluate_phase();
    if (phase == MID)
    {
        for (sq = 0; sq < 64; sq++)
        {
            piece = board[sq];
            if (piece == EMPTY)
                continue;
            switch (piece)
            {
            case WHITE_PAWN:
                table_scores += pawn_value[sq];
                break;
            case WHITE_QUEEN:
                table_scores += 0;
                break;
            case WHITE_KING:
                table_scores += king_value_open[sq];
                break;
            case BLACK_PAWN:
                table_scores -= pawn_value[FLIP(sq)];
                break;
            case BLACK_QUEEN:
                table_scores -= 0;
                break;
            case BLACK_KING:
                table_scores -= king_value_open[FLIP(sq)];
                break;
            case BLACK_BISHOP:
                if (bishops_black == 2)
                {
                    table_scores -= bishop_value[FLIP(sq)] * 1.1;
                }
                else if (bishops_black == 1)
                {
                    table_scores -= bishop_value[FLIP(sq)];
                }
                break;
            case WHITE_BISHOP:
                if (bishops_white == 2)
                {
                    table_scores += bishop_value[(sq)] * 1.1;
                }
                else if (bishops_white == 1)
                {
                    table_scores += bishop_value[(sq)] ;
                }
                break;
            case BLACK_ROOK:
                if (pawns_white + pawns_black >= 10)
                {
                    // table_scores -= rook_value_cluttered[FLIP(sq)];
                }
                else
                {
                    // table_scores -= rook_value_free[FLIP(sq)];
                }
                break;
            case WHITE_ROOK:
                if (pawns_white + pawns_black >= 10)
                {
                    // table_scores += rook_value_cluttered[(sq)];
                }
                else
                {
                    // table_scores += rook_value_free[(sq)];
                }
                break;
            default:
                break;
            }
        }
    }
    else if (phase == END)
    {
        for (sq = 0; sq < 64; sq++)
        {
            piece = board[sq];
            if (piece == EMPTY)
                continue;
            switch (piece)
            {
            case WHITE_PAWN:
                table_scores += pawn_value[sq];
                break;
            case WHITE_QUEEN:
                // table_scores += queen_value_end[sq];
                break;
            case WHITE_KING:
                table_scores += king_value_end[sq];
                break;
            case BLACK_PAWN:
                table_scores -= pawn_value[FLIP(sq)];
                break;
            case BLACK_QUEEN:
                // table_scores -= queen_value_end[FLIP(sq)];
                break;
            case BLACK_KING:
                table_scores -= king_value_end[FLIP(sq)];
                break;
            case BLACK_BISHOP:
                if (bishops_black == 2)
                {
                    table_scores -= (bishop_value[FLIP(sq)] + 10);
                }
                else if (bishops_black == 1)
                {
                    table_scores -= bishop_value[FLIP(sq)];
                }
                break;
            case WHITE_BISHOP:
                if (bishops_white == 2)
                {
                    table_scores += bishop_value[(sq)] + 10;
                }
                else if (bishops_white == 1)
                {
                    table_scores += bishop_value[(sq)];
                }
                break;
            case BLACK_ROOK:
                if (pawns_white + pawns_black >= 10)
                {
                    // table_scores -= rook_value_cluttered[FLIP(sq)];
                }
                else
                {
                    // table_scores -= rook_value_free[FLIP(sq)];
                }
                break;
            case WHITE_ROOK:
                if (pawns_white + pawns_black >= 10)
                {
                    // table_scores += rook_value_cluttered[(sq)];
                }
                else
                {
                    // table_scores += rook_value_free[(sq)];
                }
                break;

            default:
                break;
            }
        }
    }
    else if (phase == OPEN)
    {
        for (sq = 0; sq < 64; sq++)
        {
            piece = board[sq];
            if (piece == EMPTY)
                continue;
            switch (piece)
            {
            case WHITE_PAWN:
                table_scores += pawn_value[sq];
                break;
            case WHITE_QUEEN:
                // table_scores += queen_value_open[sq];
                break;
            case WHITE_KING:
                table_scores += king_value_open[sq];
                break;
            case BLACK_PAWN:
                table_scores -= pawn_value[FLIP(sq)];
                break;
            case BLACK_QUEEN:
                // table_scores -= queen_value_open[FLIP(sq)];
                break;
            case BLACK_KING:
                table_scores -= king_value_open[FLIP(sq)] / 100;
                break;
            case BLACK_BISHOP:
                if (bishops_black == 2)
                {
                    table_scores -= (bishop_value[FLIP(sq)] + 10);
                }
                else if (bishops_black == 1)
                {
                    table_scores -= bishop_value[FLIP(sq)];
                }
                break;
            case WHITE_BISHOP:
                if (bishops_white == 2)
                {
                    table_scores += (bishop_value[(sq)] + 10);
                }
                else if (bishops_white == 1)
                {
                    table_scores += bishop_value[(sq)] ;
                }
                break;
            case BLACK_ROOK:
                if (pawns_white + pawns_black >= 10)
                {
                    // table_scores -= rook_value_cluttered[FLIP(sq)];
                }
                else
                {
                    //   table_scores -= rook_value_free[FLIP(sq)];
                }
                break;
            case WHITE_ROOK:
                if (pawns_white + pawns_black >= 10)
                {
                    // table_scores += rook_value_cluttered[(sq)];
                }
                else
                {
                    // table_scores += rook_value_free[(sq)];
                }
                break;
            default:
                break;
            }
        }
    }
    table_scores = table_scores;
    score += (white_has - black_has + table_scores);
    return WTM ? score : -score;
}
/*Search Functions*/
int qsearch(int alpha, int beta)
{
    int best_score;
    int score;
    s_move *moves;
    best_score = evaluate();
    if (best_score >= beta)
        return best_score;
    moves = move_sp;
    generate_moves(PRESCORE_EQUAL + (1 << 9));
    qsort(moves, move_sp - moves, sizeof(*moves), cmp_move);
    while (move_sp > moves)
    {
        int move;
        move_sp--;
        move = move_sp->move;
        make_move(move);
        compute_attacks();
        if (friend->attack[enemy->king])
        {
            unmake_move();
            continue;
        }
        score = - qsearch(-beta, -alpha);
        unmake_move();
        if (score <= best_score)
            continue;
        best_score = score;
        if (score <= alpha)
            continue;
        alpha = score;
        if (score < beta)
            continue;
        move_sp = moves;
    }
    return best_score;
}
int search(int depth, int alpha, int beta)
{
    int best_score = -INF;
    int best_move = 0;
    int score;
    s_move *moves;
    int incheck = 0;
    incheck = enemy->attack[friend->king];
    moves = move_sp;
    generate_moves(0);
    best_move = 0;
    qsort(moves, move_sp - moves, sizeof(*moves), cmp_move);
    while (move_sp > moves)
    {
        int newdepth;
        int move;
        move_sp--;
        move = move_sp->move;
        make_move(move);
        compute_attacks();
        if (friend->attack[enemy->king])
        {
            unmake_move();
            continue;
        }
        newdepth = incheck ? depth : depth - 1;
        if (newdepth <= 0)
            score = -qsearch(-beta, -alpha);
        else
            score = -search(newdepth, -beta, -alpha);
        if (score < -29000) score++;
        unmake_move();
        if (score <= best_score) continue;
        best_score = score;
        best_move = move;
        if (score <= alpha) continue;
        alpha = score;
        if (score < beta) continue;
        move_sp = moves;
    }
    if (best_score == -INF)
    {
        if (incheck)
            best_score = -30000;
        else
            best_score = 0;
    }
    return best_score;
}
int root_search(int maxdepth)
{
    int depth;
    int score, best_score;
    int move = 0;
    int alpha, beta;
    s_move     *m;
    generate_moves(0);
    qsort(move_stack, move_sp - move_stack, sizeof(s_move), cmp_move);
    alpha = -INF;
    beta = +INF;
    for (depth = 1; depth <= maxdepth; depth++)
    {
        m = move_stack;
        best_score = -INF;
        while (m < move_sp)
        {
            make_move(m->move);
            compute_attacks();
            if (friend->attack[enemy->king] != 0)
            {
                unmake_move();
                *m = *--move_sp;
                continue;
            }
            if (depth - 1 > 0)
                score = -search(depth - 1, -beta, -alpha);
            else
                score = -qsearch(-beta, -alpha);
            unmake_move();
            if (score >= beta || (score <= alpha && m == move_stack))
            {
                alpha = -INF;
                beta = +INF;
                continue;
            }
            if (score > best_score)
            {
                s_move tmp;
                best_score = score;
                alpha = score;
                beta = score + 1;
                move = m->move;
                tmp = *move_stack;
                *move_stack = *m;
                *m = tmp;
            }
            m++;
        }
        if (move_sp - move_stack <= 1)
            break;
        qsort(move_stack + 1, move_sp - move_stack - 1, sizeof(*m), cmp_move);
        alpha = best_score - 33;
        beta = best_score + 33;
    }
    move_sp = move_stack;
    return move;
}
/*Commands to the engine*/
void cmd_default(char *s)
{
    int move;
    move = parse_move(s);
    if (move)
    {
        make_move(move);
        print_board();
    }
    else
        printf("no such move or command: %s\n", s);
}
void cmd_both(char *dummy)
{
    computer[0] = 1;
    computer[1] = 1;
}
void cmd_white(char *dummy)
{
    computer[0] = 0;
    computer[1] = 1;
    ply = 0;
    reset();
}
void cmd_black(char *dummy)
{
    computer[0] = 1;
    computer[1] = 0;
    ply = 1;
    reset();
}
void cmd_force(char *dummy)
{
    computer[0] = 0;
    computer[1] = 0;
}
void cmd_new(char *dummy)
{
    reset();
    print_board();
    computer[0] = 0;
    computer[1] = 1;
}
void cmd_quit(char *dummy)
{
    exit(0);
}
command plague_commands[] =
{
    { "new",       cmd_new},
    { "quit",      cmd_quit,},
    { "both",      cmd_both},
    { "force",     cmd_force},
    { "white",     cmd_white},
    { "black",     cmd_black},
    { "quit",      cmd_quit},
    { NULL,        cmd_default},
};
int main()
{
    int cmd;
    char name[128];
    int move;
    king_step[1 << 0] = 1;
    king_step[1 << 1] = 9;
    king_step[1 << 2] = 8;
    king_step[1 << 3] = 7;
    king_step[1 << 4] = -1;
    king_step[1 << 5] = -9;
    king_step[1 << 6] = -8;
    king_step[1 << 7] = -7;
    knight_jump[1 << 0] = -6;
    knight_jump[1 << 1] = 10;
    knight_jump[1 << 2] = 17;
    knight_jump[1 << 3] = 15;
    knight_jump[1 << 4] = -10;
    knight_jump[1 << 5] = 6;
    knight_jump[1 << 6] = -15;
    knight_jump[1 << 7] = -17;
    castle[A1] = CASTLE_WHITE_QUEEN;
    castle[E1] = CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN;
    castle[H1] = CASTLE_WHITE_KING;
    castle[A8] = CASTLE_BLACK_QUEEN;
    castle[E8] = CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN;
    castle[H8] = CASTLE_BLACK_KING;
    cmd_new(NULL);
    while (TRUE)
    {

        gets(name);
        
        for (cmd = 0; plague_commands[cmd].name != NULL; cmd++)
        {
            if (0 == strcmp(plague_commands[cmd].name, name))
                break;
        }
        plague_commands[cmd].cmd(name);
        while (computer[!WTM])
        {
            move = root_search(maxdepth);
            if (!move)
            {
                printf("game over: ");
                compute_attacks();
                if (!move && enemy->attack[friend->king] != 0)
                    puts(WTM ? "0-1\n" : "1-0\n");
                else
                    puts("1/2-1/2\n");
                computer[0] = 0;
                computer[1] = 0;
                break;
            }
            make_move(move);
            print_board();
        }
    }
    return 0;
}
