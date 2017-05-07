import pexpect
import random
import sys

# Generate pseudo-random Khet positions.
class PositionGenerator:
    def generate(self):
        pieces_to_place = ['P','P','P','P','P','P','P','A','A','S','S','K']
        board = ['' for i in range(0, 80)]
        orientations = [0 for i in range(0, 80)]

        # Place the sphinxes with the typical locations and orientations.
        board[9] = 'X'
        board[70] = 'x'
        orientations[70] = 2

        # Place the remaining pieces randomly.
        while pieces_to_place:
            loc = random.randint(0, 79)

            # Is it possible to place a silver piece there?
            if board[loc] or loc % 10 == 0 or loc == 8 or loc == 78 or loc == 9:
                continue

            # Choose and place random piece.
            p_idx = random.randint(0, len(pieces_to_place)-1)
            p = pieces_to_place.pop(p_idx)
            board[loc] = p

            # Choose a random orientation.
            o = random.randint(0, 3)
            orientations[loc] = o

            # Place the mirrored red piece.
            board[79-loc] = p.lower()
            orientations[79-loc] = o+2 if o < 2 else o-2

        return self.__khet_str(board, orientations)

    def check_balanced(self, start_pos, path_to_ai1, path_to_ai2, time_to_check):
        ai1 = AIProcess(path_to_ai1)
        ai2 = AIProcess(path_to_ai2)

        balanced_threshold = 500 # Equivalent to 1/2 pyramids.

        balanced = True

        if ai1.start() and ai2.start():
            ai1.new_game(start_pos)
            ai2.new_game(start_pos)

            ai1.search(time_to_check)
            ai2.search(time_to_check)

            balanced = not ai1.mate_detected and not ai2.mate_detected and \
                       not abs(ai1.latest_eval) > balanced_threshold and \
                       not abs(ai2.latest_eval) > balanced_threshold

        ai1.quit()
        ai2.quit()

        return balanced

    def __khet_str(self, board, orientations):
        ks = ""
        for r in range(7, -1, -1):
            space = 0
            for c in range(0, 10):
                i = 10*r + c
                if board[i]:
                    if space > 0:
                        ks += str(space)
                        space = 0

                    p = board[i];
                    ks += p
                    if p.upper() != 'K':
                        ks += str(orientations[i]);
                else:
                    space += 1

            if space > 0 and space < 10:
                ks += str(space)
                space = 0

            if r > 0:
                ks += '/'

        ks += " 0"

        return ks

# Handle communications with the AI program at the specified path.
class AIProcess:
    def __init__(self, ai_path):
        self.path = ai_path
        self.proc = None
        self.new_game()

    def start(self):
        if self.proc == None:
            self.proc = pexpect.spawn(self.path)
        return self.proc != None

    def new_game(self, start_pos = "standard"):
        if self.proc:
            self.proc.sendline("newgame")
        self.start_pos = start_pos
        self.zeroes = 0
        self.latest_eval = 0
        self.moves = ""
        self.no_moves_available = False
        self.mate_detected = False

    def quit(self):
        self.proc.sendline("stop")
        self.proc.sendline("quit")

    def add_move(self, move):
        self.moves += move + " "

    def is_likely_draw(self):
        return self.zeroes > 2

    def search(self, time):
        self.proc.sendline("position " + self.start_pos + " " + self.moves)
        self.proc.sendline("go movetime " + str(time))
        current_eval = 0
        best_move = ""
        score_type = "score"
        while not best_move:
            line = self.proc.readline().strip()
            if line.startswith("info"):
                tokens = line.split(" ")
                if "score" in tokens:
                    i = tokens.index("score")
                    current_eval = int(tokens[i+1])
                elif "mate" in tokens:
                    i = tokens.index("mate")
                    score_type = "mate"
                    current_eval = int(tokens[i+1])
                    self.mate_detected = True
            elif line.startswith("bestmove"):
                tokens = line.split(" ")
                best_move = tokens[1]

        self.zeroes += 1 if current_eval == 0 else 0

        self.latest_eval = current_eval
        self.add_move(best_move)
        self.no_moves_available = best_move == "none"

        return [best_move, score_type, self.latest_eval]

def get_human_move():
    move = ""
    while not move:
        move = raw_input("What is your move?")
        sure = raw_input("Are you sure? (no backsies!)")
        move = move if sure.upper() == "Y" else ""
        move = move if sure.upper() != "Q" else "quit"
    return move

# Play human vs AI game.
def human_vs_ai(human_first, path_to_ai, time_per_move):
    ai = AIProcess(path_to_ai)
    quit = False
    if ai.start():
        if human_first:
            move = get_human_move()
            quit = move == "quit"
            ai.add_move(move)

        while not quit:
            ai_move = ai.search(time_per_move)
            print ai_move

            move = get_human_move()
            ai.add_move(move)
            quit = move == "quit"

        ai.quit()
    else:
        print "Error starting AI process"

# Play AI vs AI game.
def ai_vs_ai(path_to_ai1, path_to_ai2, time_per_move, start_pos = "standard"):
    ai1 = AIProcess(path_to_ai1)
    ai2 = AIProcess(path_to_ai2)

    one_win = False
    two_win = False
    draw = False

    win_threshold = 3000 # Equivalent to 3 pyramids.

    if ai1.start() and ai2.start():
        ai1.new_game(start_pos)
        ai2.new_game(start_pos)

        terminal = False
        while not terminal:
            ai_move = ai1.search(time_per_move)
            print "AI1:", ai_move
            if ai1.no_moves_available:
                if ai1.mate_detected:
                    one_win = ai1.latest_eval > 0
                    two_win = not one_win
                else:
                    draw = True
                break

            ai2.add_move(ai_move[0])

            ai_move = ai2.search(time_per_move)
            print "AI2:", ai_move
            if ai2.no_moves_available:
                if ai2.mate_detected:
                    one_win = ai2.latest_eval > 0
                    two_win = not one_win
                else:
                    draw = True
                break

            ai1.add_move(ai_move[0])

            one_win = ai1.latest_eval > win_threshold and ai2.latest_eval > win_threshold
            two_win = ai1.latest_eval < -win_threshold and ai2.latest_eval < -win_threshold
            draw = ai1.is_likely_draw() and ai2.is_likely_draw()

            terminal = one_win or two_win or draw

    return 1 if one_win else -1 if two_win else 0

# Run a tournament between the two programs and report the results.
def ai_tournament(path_to_ai1, path_to_ai2, num_positions, time_per_move):
    gen = PositionGenerator()
    one_points = 0
    games = 0
    for p in range(0, num_positions):
        # Generate a (probably balanced) initial position.
        balanced = False
        while not balanced:
            pos = gen.generate()
            balanced = gen.check_balanced(pos, path_to_ai1, path_to_ai2, time_per_move)

        print pos

        # The AI's much play both sides of this position.
        score = ai_vs_ai(path_to_ai1, path_to_ai2, time_per_move, pos)
        if score > 0:
            one_points += 1
        elif score == 0:
            one_points += 0.5
        games += 1

        score = ai_vs_ai(path_to_ai2, path_to_ai1, time_per_move, pos)
        if score < 0:
            one_points += 1
        elif score == 0:
            one_points += 0.5
        games += 1

        print (one_points + .0)/games, one_points, "/", games

# Usage:
# runner.py human ai path-to-khet-ai time-per-move (human is silver vs ai)
# runner.py ai human path-to-khet-ai time-per-move (human is red vs ai)
# runner.py ai ai path-to-khet-ai-1 path-to-khet-ai-2 time-per-move (ai vs ai)
# runner.py tournament path-to-khet-ai1 path-to-khet-ai-2 time-per-move
if __name__ == "__main__":
    num_args = len(sys.argv)
    if num_args == 5:
        if sys.argv[1] == "tournament":
            ai_tournament(sys.argv[2], sys.argv[3], 1000, sys.argv[4])
        else:
            human_first = sys.argv[1] == "human"
            human_vs_ai(human_first, sys.argv[3], sys.argv[4])
    elif num_args == 6:
        ai_vs_ai(sys.argv[3], sys.argv[4], sys.argv[5])
    else:
        print "Wrong number of arguments"
