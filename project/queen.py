import sys

size = 4 if len(sys.argv) < 2 else int(sys.argv[1])

def init_board(size):
   return [ i + 1 for i in range(size) ]

def check_conflict(board, idx):
    for i in range(idx):
        if abs(board[idx] - board[i]) == idx - i:
            return True
    return False

def permute(board, idx):
   count = 0
   size = len(board)
   if idx >= 2:
       if check_conflict(board, idx-1):
           return 0
   if idx == size-1:
       if not check_conflict(board, idx):
           count += 1
#          print(board)
       return count
   count += permute(board, idx+1)
   for i in range(idx+1, size):
      tmp = board[idx]
      board[idx] = board[i]
      board[i] = tmp
      count += permute(board, idx+1)
   tmp = board[idx]
   for i in range(idx, size-1):
      board[i] = board[i+1]
   board[size-1] = tmp
   return count

board = init_board(size)
count = permute(board, 0)
print(count)
