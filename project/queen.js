console.log(process.argv);
let size = process.argv.length > 2 ? Number(process.argv[2]) : 4;
board = init_board(size);
let count = permute(board, 0);
console.log(count);

function init_board(size) {
   let arr = new Array(size);
   for (let i = 0; i < size; i++) arr[i] = i+1;
   return arr;
}

function check_conflict(board, idx) {
    for (i = 0; i < idx; i++) {
        if (Math.abs(board[idx] - board[i]) === idx - i) {
            return true;
        }
    }
    return false;
}

function permute(board, idx) {
   let count = 0;
   let size = board.length;
   if (idx >= 2) {
       if (check_conflict(board, idx-1)) return 0;
   }
   if (idx == size-1) {
       if (!check_conflict(board, idx)) {
           count++;
//           console.log(board);
       }
       return count;
   }
   count += permute(board, idx+1);
   for (let i = idx+1; i < size; i++) {
      tmp = board[idx];
      board[idx] = board[i];
      board[i] = tmp;
      count += permute(board, idx+1);
   }
   tmp = board[idx];
   for (let i = idx; i < size - 1; i++) {
      board[i] = board[i+1];
   }
   board[size-1] = tmp;
   return count;
}
