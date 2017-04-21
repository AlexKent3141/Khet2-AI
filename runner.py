import pexpect
import sys

class AIProcess:
    def __init__(self, ai_path):
        self.path = ai_path
        self.proc = None
        self.zeroes = 0
        self.latest_eval = 0
        self.moves = ""

    def start(self):
        self.proc = pexpect.spawn(self.path)
        return self.proc != None

    def quit(self):
        self.proc.sendline("stop")
        self.proc.sendline("quit")

    def add_move(self, move):
        self.moves += move + " "

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
            elif line.startswith("bestmove"):
                tokens = line.split(" ")
                best_move = tokens[1]

        if current_eval == 0:
            self.zeroes += 1
        else:
            self.zeroes = 0

        self.latest_eval = current_eval
        self.add_move(best_move)

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
    pass

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
