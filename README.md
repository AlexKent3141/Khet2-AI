# Khet2-AI
AI program which plays Khet 2.0.

Khet 2.0 is an interesting two-player board game where each player tries to position their pieces (some of which have reflective sides) so that when they fire their laser the opponent's Pharaoh is hit.

At the time of writing there isn't much established theory for Khet 2.0 so this work will be fairly experimental.

# Rules
The rules for Khet 2.0 can be found on the official website: http://khet.com/.

For the purposes of this project I will make the following restrictions and extensions:
1) The Pharaoh cannot rotate: this seems like a sensible restriction as in general this would not change the board state. Even if the board state would change (a piece will be "lasered" passively) I expect there to always be a more productive move available.
2) Scarabs can only rotate in one direction (clockwise): both Scarab rotation moves are equivalent, so it makes sense to restrict to one of them.
3) Threefold repetition: as defined in chess, if the same position is repeated three times then the game is drawn.
4) Fifty move rule: similar to the chess rule, if no pieces are captured in fifty moves then the game is drawn.

These may be subject to further tweaks once the AI can be tested fully.

# Roadmap
1) Decide on board representation *Done*
2) Implement move generation (with basic testing framework) *Done*
3) Implement make/unmake move functionality *Done*
4) Implement basic AI (iterative deepening alpha-beta) *In Progress*
5) Implement communications (see UCI protocol for chess)
6) ...
