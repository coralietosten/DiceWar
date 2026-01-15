#include "Proba.h"

double probabilite_victoire(int attackerDice, int defenderDice) 
{
    if (attackerDice <= 1 )
    {
        return 0.00;
    }
    if (attackerDice >= 8 )
    {
        attackerDice = 8;
    }
    if (defenderDice <= 1 )
    {
        defenderDice = 1;
    }
    if (defenderDice >= 8 )
    {
        defenderDice = 8;
    }
    
    // Attacker = 2
    if (attackerDice == 2 && defenderDice == 2) return 44.37;
    if (attackerDice == 2 && defenderDice == 3) return 15.20;
    if (attackerDice == 2 && defenderDice == 4) return 3.59;
    if (attackerDice == 2 && defenderDice == 5) return 0.61;
    if (attackerDice == 2 && defenderDice == 6) return 0.08;
    if (attackerDice == 2 && defenderDice == 7) return 0.01;
    if (attackerDice == 2 && defenderDice == 8) return 0.00;

    // Attacker = 3
    if (attackerDice == 3 && defenderDice == 2) return 77.85;
    if (attackerDice == 3 && defenderDice == 3) return 45.36;
    if (attackerDice == 3 && defenderDice == 4) return 19.17;
    if (attackerDice == 3 && defenderDice == 5) return 6.07;
    if (attackerDice == 3 && defenderDice == 6) return 1.49;
    if (attackerDice == 3 && defenderDice == 7) return 0.29;
    if (attackerDice == 3 && defenderDice == 8) return 0.05;

    // Attacker = 4
    if (attackerDice == 4 && defenderDice == 2) return 93.92;
    if (attackerDice == 4 && defenderDice == 3) return 74.28;
    if (attackerDice == 4 && defenderDice == 4) return 45.95;
    if (attackerDice == 4 && defenderDice == 5) return 22.04;
    if (attackerDice == 4 && defenderDice == 6) return 8.34;
    if (attackerDice == 4 && defenderDice == 7) return 2.54;
    if (attackerDice == 4 && defenderDice == 8) return 0.64;

    // Attacker = 5
    if (attackerDice == 5 && defenderDice == 2) return 98.79;
    if (attackerDice == 5 && defenderDice == 3) return 90.93;
    if (attackerDice == 5 && defenderDice == 4) return 71.81;
    if (attackerDice == 5 && defenderDice == 5) return 46.37;
    if (attackerDice == 5 && defenderDice == 6) return 24.24;
    if (attackerDice == 5 && defenderDice == 7) return 10.36;
    if (attackerDice == 5 && defenderDice == 8) return 3.67;

    // Attacker = 6
    if (attackerDice == 6 && defenderDice == 2) return 99.82;
    if (attackerDice == 6 && defenderDice == 3) return 97.53;
    if (attackerDice == 6 && defenderDice == 4) return 88.40;
    if (attackerDice == 6 && defenderDice == 5) return 69.96;
    if (attackerDice == 6 && defenderDice == 6) return 46.67;
    if (attackerDice == 6 && defenderDice == 7) return 26.00;
    if (attackerDice == 6 && defenderDice == 8) return 12.15;

    // Attacker = 7
    if (attackerDice == 7 && defenderDice == 2) return 99.98;
    if (attackerDice == 7 && defenderDice == 3) return 99.47;
    if (attackerDice == 7 && defenderDice == 4) return 96.15;
    if (attackerDice == 7 && defenderDice == 5) return 86.24;
    if (attackerDice == 7 && defenderDice == 6) return 68.52;
    if (attackerDice == 7 && defenderDice == 7) return 46.91;
    if (attackerDice == 7 && defenderDice == 8) return 27.44;

    // Attacker = 8
    if (attackerDice == 8 && defenderDice == 2) return 100.00;
    if (attackerDice == 8 && defenderDice == 3) return 99.91;
    if (attackerDice == 8 && defenderDice == 4) return 98.95;
    if (attackerDice == 8 && defenderDice == 5) return 94.77;
    if (attackerDice == 8 && defenderDice == 6) return 84.39;
    if (attackerDice == 8 && defenderDice == 7) return 67.35;
    if (attackerDice == 8 && defenderDice == 8) return 47.11;

    return 50.00;  // au cas où
}
