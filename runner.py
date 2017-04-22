import pexpect
import sys

class AIProcess:
    def __init__(self, ai_path):
        self.path = ai_path
        self.proc = None
        self.new_game()

    def start(self):
        self.proc = pexpect.spawn(self.path)
        return self.proc != None

    def new_game(self):
        if self.proc:
            self.proc.sendline("newgame")
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
        self.proc.sendline("position standard " + self.moves)
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

        if current_eval == 0:
            self.zeroes += 1
        else:
            self.zeroes = 0

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
def ai_vs_ai(path_to_ai1, path_to_ai2, time_per_move):
    ai1 = AIProcess(path_to_ai1)
    ai2 = AIProcess(path_to_ai2)

    one_win = False
    two_win = False
    draw = False

    win_threshold = 3000 # Equivalent to 3 pyramids.

    if ai1.start() and ai2.start():
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

            one_win = ai2.latest_eval > win_threshold
            two_win = ai2.latest_eval < -win_threshold
            draw = ai1.is_likely_draw() and ai2.is_likely_draw()

            terminal = one_win or two_win or draw

# Usage:
# runner.py human ai path-to-khet-ai time-per-move (human is silver vs ai)
# runner.py ai human path-to-khet-ai time-per-move (human is red vs ai)
# runner.py ai ai path-to-khet-ai-1 path-to-khet-ai-2 time-per-move (ai vs ai)
if __name__ == "__main__":
    num_args = len(sys.argv)
    if num_args == 5:
        human_first = sys.argv[1] == "human"
        human_vs_ai(human_first, sys.argv[3], sys.argv[4])
    elif num_args == 6:
        ai_vs_ai(sys.argv[3], sys.argv[4], sys.argv[5])
    else:
        print "Wrong number of arguments"
