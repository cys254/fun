<<__EntryPoint>>
function queen_main(): void {
    $size = (int)(vec(\HH\global_get('argv') as Container<_>)[1] ?? 4);
    $board = get_init_vecs($size);
    $count = permute($board, 0);
    echo $count, \PHP_EOL;
}

function get_init_vecs(int $size): Vector<int> {
   $arr = Vector{};
   for ($i=1; $i<=$size; $i++) {
      $arr[] = $i;
   }
   return $arr;
}

function print_board(Vector<int> $board): void {
    foreach ($board as $item) {
        echo ' ', $item;
    }
    echo \PHP_EOL;
}

function check_conflict(Vector<int> $board, int $idx): bool {
    for ($i = 0; $i < $idx; $i++) {
        if (HH\Lib\Math\abs($board[$idx] - $board[$i]) === $idx - $i) {
            return true;
        }
    }
    return false;
}

function permute(Vector<int> $board, int $idx): int {
   $count = 0;
   $size = \count($board);
   if ($idx >= 2) {
       if (check_conflict($board, $idx-1)) return 0;
   }
   if ($idx == $size-1) {
       if (!check_conflict($board, $idx)) {
           $count++;
//           print_board($board);
       }
       return $count;
   }
   $count += permute($board, $idx+1);
   for ($i = $idx+1; $i < $size; $i++) {
      $tmp = $board[$idx];
      $board[$idx] = $board[$i];
      $board[$i] = $tmp;
      $count += permute($board, $idx+1);
   }
   $tmp = $board[$idx];
   for ($i = $idx; $i < $size - 1; $i++) {
      $board[$i] = $board[$i+1];
   }
   $board[$size-1] = $tmp;
   return $count;
}
