import math
import random
import sys
import time


if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8")


# 보드는 1차원 리스트로 구현한다.
game_board = [' ', ' ', ' ',
              ' ', ' ', ' ',
              ' ', ' ', ' ']


def empty_cells(board):
    """비어 있는 칸의 위치를 리스트로 반환한다."""
    return [i for i, cell in enumerate(board) if cell == ' ']


def valid_move(x):
    """현재 게임 보드의 x 위치에 돌을 놓을 수 있는지 검사한다."""
    return x in empty_cells(game_board)


def move(x, player):
    """게임 보드의 x 위치에 player의 돌을 놓는다."""
    if valid_move(x):
        game_board[x] = player
        return True
    return False


def draw(board):
    """현재 게임 보드를 출력한다."""
    for i, cell in enumerate(board):
        if i % 3 == 0:
            print('\n----------------')
        print('|', cell, '|', end='')
    print('\n----------------')


def check_win(board, player):
    """player가 가로, 세로 또는 대각선으로 승리했는지 검사한다."""
    win_positions = (
        (0, 1, 2), (3, 4, 5), (6, 7, 8),
        (0, 3, 6), (1, 4, 7), (2, 5, 8),
        (0, 4, 8), (2, 4, 6),
    )
    return any(all(board[i] == player for i in line)
               for line in win_positions)


def game_over(board):
    """승자가 있거나 빈칸이 없으면 True를 반환한다."""
    return (check_win(board, 'X') or check_win(board, 'O')
            or not empty_cells(board))


def opponent(player):
    return 'O' if player == 'X' else 'X'


class MCTSNode:
    """몬테카를로 트리 탐색에서 사용하는 하나의 게임 상태이다."""

    def __init__(self, board, player, parent=None, move_position=None):
        self.board = board
        self.player = player              # 이 상태에서 둘 차례
        self.parent = parent
        self.move_position = move_position
        self.children = []
        self.untried_moves = empty_cells(board)
        self.visits = 0
        self.wins = 0.0

    def select_child(self, exploration=math.sqrt(2.0)):
        """UCB 값이 가장 큰 자식 노드를 선택한다."""
        return max(
            self.children,
            key=lambda child: (
                child.wins / child.visits
                + exploration
                * math.sqrt(math.log(self.visits) / child.visits)
            ),
        )

    def expand(self):
        """아직 탐색하지 않은 수 하나를 두어 자식 노드를 만든다."""
        position = random.choice(self.untried_moves)
        self.untried_moves.remove(position)

        next_board = self.board.copy()
        next_board[position] = self.player
        child = MCTSNode(
            next_board,
            opponent(self.player),
            parent=self,
            move_position=position,
        )
        self.children.append(child)
        return child


def simulate(board, player):
    """현재 상태부터 무작위로 게임을 끝까지 진행하여 승자를 반환한다."""
    simulation_board = board.copy()
    simulation_player = player

    while not game_over(simulation_board):
        position = random.choice(empty_cells(simulation_board))
        simulation_board[position] = simulation_player
        simulation_player = opponent(simulation_player)

    if check_win(simulation_board, 'X'):
        return 'X'
    if check_win(simulation_board, 'O'):
        return 'O'
    return None


def backpropagate(node, winner):
    """시뮬레이션 결과를 루트 노드까지 역전파한다."""
    while node is not None:
        node.visits += 1

        # 이 노드에 도착하기 직전에 돌을 놓은 경기자의 관점으로 기록한다.
        player_just_moved = opponent(node.player)
        if winner is None:
            node.wins += 0.5
        elif winner == player_just_moved:
            node.wins += 1.0

        node = node.parent


def mcts(board, player, simulations=2000):
    """MCTS를 수행하여 둘 위치와 그 위치의 추정 승률을 반환한다."""
    if game_over(board):
        return -1, 0.0

    root = MCTSNode(board.copy(), player)

    for _ in range(simulations):
        node = root

        # 1. 선택: 완전히 확장된 노드에서는 UCT가 가장 큰 자식을 고른다.
        while (not node.untried_moves and node.children
               and not game_over(node.board)):
            node = node.select_child()

        # 2. 확장: 아직 시도하지 않은 수 하나를 트리에 추가한다.
        if node.untried_moves and not game_over(node.board):
            node = node.expand()

        # 3. 시뮬레이션: 선택된 상태에서 무작위 게임을 수행한다.
        winner = simulate(node.board, node.player)

        # 4. 역전파: 승패 결과로 방문 횟수와 승리 점수를 갱신한다.
        backpropagate(node, winner)

    # 실제 수는 탐색 중 가장 많이 방문한 자식을 선택한다.
    best_child = max(root.children, key=lambda child: child.visits)
    win_rate = best_child.wins / best_child.visits
    return best_child.move_position, win_rate


def main():
    player = 'X'
    total_simulations = 0
    start_time = time.time()

    while True:
        draw(game_board)
        if game_over(game_board):
            break

        position, win_rate = mcts(game_board, player)
        total_simulations += 2000
        print(f"{player} 선택: {position}, 추정 승률: {win_rate:.3f}")
        move(position, player)
        player = opponent(player)

    if check_win(game_board, 'X'):
        print('X 승리!')
    elif check_win(game_board, 'O'):
        print('O 승리!')
    else:
        print('비겼습니다!')

    print("MCTS simulation #: " + str(total_simulations))
    print("---{}s seconds---".format(time.time() - start_time))


if __name__ == '__main__':
    main()
