#include <memory>
#include <vector>
#include <iostream>
#include <cstdlib>

std::shared_ptr<std::vector<int> > init_board(int size) {
    auto board = std::shared_ptr<std::vector<int> >(new std::vector<int>(size));
    for (int i = 0; i < size; i++) {
        (*board)[i] = i+1;
    }
    return board;
}

bool check_conflict(std::shared_ptr<std::vector<int> >board, int idx) {
    for (int i = 0; i < idx; i++) {
        if (std::abs((*board)[idx] - (*board)[i]) == idx - i) {
            return true;
        }
    }
    return false;
}

int permute(std::shared_ptr<std::vector<int> >board, int idx) {
   int count = 0;
   int size = board->size();
   if (idx >= 2 && check_conflict(board, idx-1)) return 0;
   if (idx == size-1) {
       if (!check_conflict(board, idx)) count++;
       return count;
   }
   count += permute(board, idx+1);
   for (int i = idx+1; i < size; i++) {
      auto tmp = (*board)[idx];
      (*board)[idx] = (*board)[i];
      (*board)[i] = tmp;
      count += permute(board, idx+1);
   }
   auto tmp = (*board)[idx];
   for (int i = idx; i < size-1; i++) {
      (*board)[i] = (*board)[i+1];
   }
   (*board)[size-1] = tmp;
   return count;
}

int main(int argc, char** argv) {
   int size = 4;
   if (argc > 1) {
        size = std::atoi(argv[1]);
   }
   auto board = init_board(size);
   std::cout << permute(board, 0) << std::endl;
}
