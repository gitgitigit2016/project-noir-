#include "globals.h"

Game G;

const char *RUMORS[TOTAL_RUMORS] = {
    "A maid says Arthur the butler threatened Mr. Blackwood after dinner.",
    "Someone saw Dr. Reed leave the dining room early and head toward the garden.",
    "Eleanor Blackwood was seen searching her husband's desk before the murder."
};

Suspect SUSPECTS[TOTAL_SUSPECTS] = {
    {
        "Eleanor Blackwood",
        "Wife",
        "She had gambling debt and secretly searched the victim's study."
    },
    {
        "Arthur Hale",
        "Butler",
        "He argued with the victim after being told he might lose his job."
    },
    {
        "Dr. Reed",
        "Family Doctor",
        "He left dinner early and tried to hide why he had been in the garden."
    },
    {
        "Victor Crowe",
        "Business Partner",
        "He handled the manor accounts and had a private meeting with the victim."
    }
};

Evidence EVIDENCE[TOTAL_EVIDENCE] = {
    {
        "Witness Statement",
        "A servant confirms Arthur argued with Mr. Blackwood at 9:20 PM.",
        3, 1, 3, false, false
    },
    {
        "Torn Letter",
        "The victim wrote: 'Victor, meet me in the study at 10. I know where the money went.'",
        6, 4, 4, false, false
    },
    {
        "Muddy Shoes",
        "Dr. Reed's shoes match the garden mud. He admits he went outside to hide a private letter.",
        8, 1, 4, false, false
    },
    {
        "Broken Watch",
        "The victim's damaged pocket watch stopped at 10:15 PM.",
        4, 4, 5, false, false
    },
    {
        "Station Ticket",
        "Arthur's ticket was checked at 10:05 PM at a station too far away to return by 10:15.",
        9, 4, 5, false, false
    },
    {
        "Autopsy Report",
        "MAJOR: Death occurred around 10:15 PM from a heavy blow. No poison was used.",
        6, 1, 7, true, false
    },
    {
        "Broken Cufflink",
        "MAJOR: A cufflink engraved 'V.C.' was found beneath the victim's desk beside fresh blood.",
        8, 4, 8, true, false
    }
};
