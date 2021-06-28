import sys
import queen_cython

size = 4 if len(sys.argv) < 2 else int(sys.argv[1])

def init_board(size):
   return [ i + 1 for i in range(size) ]

board = init_board(size)
count = queen_cython.permute(board, 0)
print(count)
