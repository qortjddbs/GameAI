# N-Queen 문제를 A* 탐색 방법으로 구현
# - 한 행(row)에 퀸을 하나씩 순서대로 배치한다고 가정한다.
# - 상태(State): 지금까지 배치된 퀸들의 열(column) 위치 리스트
#   예: [1, 3, 0] -> 0행에 1열, 1행에 3열, 2행에 0열에 퀸이 있음
# - 연산자: 다음 행에 퀸 하나를 추가한다 (열 0 ~ N-1 중 선택, 자식 노드 N개)
# - g(n): 지금까지 놓은 퀸의 수 (깊이)
# - h(n): 지금까지 놓인 퀸들 중 서로 공격하는(같은 열 또는 같은 대각선) 쌍의 개수
# - f(n) = g(n) + h(n)
# - 목표 상태: 깊이 == N 이고 h(n) == 0 (N개 퀸이 모두 배치되었고 충돌이 없음)

import queue

STUDENT_ID = "2022180016"
STUDENT_NAME = "백성윤"


class State:
    def __init__(self, board, depth=0):
        self.board = board   # 각 행에 배치된 퀸의 열 번호 리스트
        self.depth = depth   # 지금까지 배치한 퀸의 수 (= g(n))

    # 다음 행의 col번째 열에 퀸을 하나 추가한 새로운 상태를 반환한다.
    def get_new_state(self, col):
        return State(self.board + [col], self.depth + 1)

    # 자식 노드를 확장하여 리스트로 반환한다.
    def expand(self, n):
        result = []
        for col in range(n):
            result.append(self.get_new_state(col))
        return result

    # 휴리스틱 함수 h(n): 현재 배치된 퀸들 중 서로 공격하는 쌍의 개수
    def h(self):
        conflicts = 0
        for i in range(len(self.board)):
            for j in range(i + 1, len(self.board)):
                if self.board[i] == self.board[j]:           # 같은 열
                    conflicts += 1
                elif abs(self.board[i] - self.board[j]) == j - i:  # 같은 대각선
                    conflicts += 1
        return conflicts

    # g(n): 시작 노드로부터의 깊이
    def g(self):
        return self.depth

    # f(n) = g(n) + h(n)
    def f(self):
        return self.g() + self.h()

    # 상태와 상태를 비교하기 위하여 less than 연산자를 정의한다 (PriorityQueue에서 사용).
    def __lt__(self, other):
        return self.f() < other.f()

    def __str__(self):
        return f"f(n)={self.f()} g(n)={self.g()} h(n)={self.h()} board={self.board}"


# 보드를 사람이 보기 쉬운 형태로 출력한다.
def print_board(board, n):
    for row in range(n):
        line = ['X'] * n
        line[board[row]] = 'Q'
        print(' '.join(line))


def solve_n_queen(n):
    open_queue = queue.PriorityQueue()
    open_queue.put(State([], 0))

    while not open_queue.empty():
        current = open_queue.get()

        # 목표 상태 검사: N개의 퀸이 모두 배치되었고, 충돌이 없다.
        if current.depth == n and current.h() == 0:
            return current

        # N개보다 적게 배치되었으면 다음 행에 퀸을 추가하여 확장한다.
        if current.depth < n:
            for child in current.expand(n):
                open_queue.put(child)

    return None  # 해가 없는 경우 (N=2, N=3)


if __name__ == "__main__":
    n = int(input("N을 입력하세요: "))
    result = solve_n_queen(n)

    print(f"학번: {STUDENT_ID}  이름: {STUDENT_NAME}")

    if result:
        print(f"탐색 성공 (N={n})")
        print_board(result.board, n)
    else:
        print(f"N={n}에 대한 해가 존재하지 않습니다.")