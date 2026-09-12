# 2022180016 백성윤 - 게임 인공지능 과제 2
import queue

# 상태 : 지금까지 몇 개의 퀸을 놓았고, 각각 몇 번째 열에 놓았는가
class State:
    def __init__(self, board, depth=0):
        self.board = board   # 각 행에 배치된 퀸의 열 번호 리스트
        self.depth = depth   # 지금까지 배치한 퀸의 수

    # 다음 행의 col번째 열에 퀸을 하나 추가한 새로운 상태를 반환
    def get_new_state(self, col):
        return State(self.board + [col], self.depth + 1)

    # 다음 행에 퀸을 놓을 수 있는 모든 경우의 수
    def expand(self, n):
        result = []
        for col in range(n):
            result.append(self.get_new_state(col))
        return result

    # h(n): 현재 배치된 퀸들 중 서로 공격하는 쌍의 개수
    def h(self):
        conflicts = 0
        for i in range(len(self.board)):
            for j in range(i + 1, len(self.board)):
                if self.board[i] == self.board[j]:           # 퀸이 같은 열에 있다면
                    conflicts += 1
                elif abs(self.board[i] - self.board[j]) == j - i:  # 퀸이 같은 대각선에 있다면
                    conflicts += 1
        return conflicts

    # g(n): 시작 노드로부터의 깊이
    def g(self):
        return self.depth

    # f(n) = g(n) + h(n)
    def f(self):
        return self.g() + self.h()

    # PriorityQueue는 내부적으로 heap을 사용하기 때문에 크기 비교를 위해 < 연산자 오버로딩 필요
    def __lt__(self, other):
        return self.f() < other.f()

    def __str__(self):
        return f"f(n)={self.f()} g(n)={self.g()} h(n)={self.h()} board={self.board}"


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

        # 목표 상태 검사: N개의 퀸이 모두 배치되었고, 충돌이 없다면
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

    print(f"학번: 2022180016  이름: 백성윤")

    if result:
        print(f"탐색 성공 (N={n})")
        print_board(result.board, n)
    else:
        print(f"N={n}에 대한 해가 존재하지 않습니다.")