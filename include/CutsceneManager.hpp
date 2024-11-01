#ifndef CUTSCENEMANAGER_HPP
#define CUTSCENEMANAGER_HPP
#include "Logic.hpp"
#include <iostream>
#include <fstream>

class CutsceneManager {
    private:
        //std::vector<int> tutorialMoves; //= { 0, 8,    // Owner moves
        //                             0, 8, 
        //                             0, 8, 
        //                             0, 6, 
        //                             0, 15, 25, 25,  // Rodent moves left, "Owner: Hey fluffster, how are you settling in?"
        //                             0, 15,          // Rodent moves 
        //                             0, 25, 25,      // "Owner: All those years of studying and training, then eighteen long, long months in that tin can..."
        //                             0, 14,          // Rodent moves 
        //                             0, 25, 25,      // "Owner: I'm so happy to finally be here and get to work. Everyone back on Earth is going to love our new home."
        //                             0, 14,          // Rodent moves 
        //                             0, 25, 25,      // "Owner: Just look around! Isn't it beautiful?"
        //                             0, 14,          // Rodent moves 
        //                             0, 16, 
        //                             0, 14, 
        //                             0, 19,          // Rodent bites
        //                             0, 13,          // Rodent moves 
        //                             0, 15, 
        //                             0, 13, 
        //                             0, 25, 25,      // "Owner: ARGHH! What the FUCK was that?!?!"
        //                             0, 13,          // Rodent moves 
        //                             0, 13, 
        //                             0, 16, 
        //                             0, 16, 
        //                             0, 16,          // Rodent back at starting space
        //                             0, 10,          // Janelle moves (enters view)
        //                             0, 25, 25,      // "Janelle: Rats!"
        //                             0, 10,          // Janelle moves 
        //                             0, 10, 
        //                             0, 10, 
        //                             0, 12, 
        //                             0, 10,
        //                             0, 10,
        //                             0, 25, 25,      // "Owner: No shit, rats! What was that thing?"
        //                             0, 25, 25,      // "Janelle: Rats, literally. Some kind of alien rodent. Very aggressive. Seems to be some sort of disease."
        //                             0, 25, 25,      // "Janelle: Derek and I have been observing them. Other life forms here tend to steer clear."
        //                             0, 25, 25,      // "Owner: Hold on - diseased? That thing just bit me!"
        //                             0, 25, 25,      // "Janelle: Yes. Very, very unfortunate. I was afraid this might happen. The good news is Derek and Seb have begun developing a cure."
        //                             0, 25, 25,      // "Janelle: The bad news is the rodents have infested our supply decks and have blocked off the route to critical ingredients."
        //                             0, 25, 25,      // "Janelle: Now that you are infected, it is imperative that we retrieve these missing ingredients."
        //                             0, 17,          // Cat scratch
        //                             0, 25, 25,      // "Janelle: Oh! A cat! Of course! (How did I not think of that...?) Your cat is the perfect candidate for such a mission."
        //                             0, 25, 25,      // "Owner: What do you say, pooks? Down to go chase some mice?"
        //                             0, 18,          // Cat pet
        //                             0, 25, 25,      // "Janelle: That's great! To finish the cure, you'll need to collect a BEAKER, a THERMOMETER, a PLANT, and a VIAL OF BLOOD."
        //                             0, 25, 25,      // "Owner: A vial of blood?"
        //                             0, 25, 25,      // "Janelle: Yeah, it's Seb's. She misplaced it. We think it contains some critical antibodies. She's built different."
        //                             0, 25, 25,      // "Janelle: Okay, cat, here's what we know:"
        //                             0, 25, 25,      // "Janelle: The rodents are pretty much deaf. If they can't see you, they won't come after you. You can use the supply crates to stay hidden."
        //                             0, 25, 25,      // "Janelle: Press E to SCRATCH a rodent."
        //                             0, 25, 25,      // "Janelle: Press I to pick up ITEMS and TALK to my research team. They're smart cookies. Plenty of knowledge to gain from them."
        //                             0, 25, 25,      // "Janelle: If you forget any of this, you can press ESC to PAUSE, where you'll find our report on your abilities and how to control them."
        //                             0, 25, 25,      // "Janelle: Good luck!"
        //                             0, 9,           // Janelle moves (leaves)
        //                             0, 9, 
        //                             0, 11, 
        //                             0, 9, 
        //                             0, 9, 
        //                             0, 9, 
        //                             0, 9, 
        //                             0, 25, 25,      // "Owner: Man, I'm so glad they let me bring you with me."
        //                             0, 25, 25,      // "Owner: I believe in you, pooks. Go get 'em!"
        //                             0, 26           // end
        //                         };

        std::unordered_map<int, std::vector<int>> cutsceneIdtoCutsceneActionList;


        int preBossMovesSize = 2;
        int preBossMoves[2] = {0, 1};

        int cutsceneId = 0; // the corresponding cutscene, 0 = tutorial
        // Controls delay between next cutscene action
        double cutsceneActionCooldown = .25; // delayed called by 0 in action lists
        double cutsceneElapsedTimeSinceAction = 0.0;
        int cutsceneIndex = 0;

        // Dialogue conditionals
        bool isPaused;
        bool isQuit;
        std::string talk;

        // Individual Cutscenes: Collection of actions
        std::string handleNextCutsceneAction(std::shared_ptr<Logic> logic, int deltaMs, SDL_Event e, std::vector<int> cutsceneActions);

    public:
        CutsceneManager();

        void setIsPaused(bool paused) { this->isPaused = paused; };
        void setIsQuit(bool quit) { this->isQuit = quit; };
        void setCutsceneId(int s) {this->cutsceneId = s; };

        bool getIsPaused() { return isPaused; };
        bool getIsQuit() { return isQuit; };
        
        void readCutsceneActionLists();

        std::string handleCutscene(std::shared_ptr<Logic> logic, int deltaMs, SDL_Event e);
};

#endif