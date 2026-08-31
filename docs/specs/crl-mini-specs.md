# Cycling Rankings Library Mini Specs

This documents shortly introduces mini specifications of library **Cycling Rankings Library**.

What are minimalist specs?

First, they constitue a version 0 of a specifications document - notice: this document may remain in version 0, with no further enhancement.

Second, they are informal descriptions of :

- what the library is for, and what it is not for;
- what are the different components of the library;
- how those components are related to other library components;
- maybe how they could/should be implemented in the different targetted programmation languages;
- how the library components may/should be connected to the calling applications codes.

These minimalist specs are described in this single MarkDown document `crl-mini-specs.md`.


## 1. What the library is for

First, let's state what **Cycling Rankings Library** is not for: it does not prepare any display of rankings; it does not help for the management of data on disk or on the cloud either (in files or databases). This is left to the responsability of the calling applications.

**Cycling Rankings Library** only deals with the management of rankings related to cycling activities : individuals as well as teams rankings, evaluating dialy and general rankings for time or points classifications. 

**Cycling Rankings Library** aims at being usable for true cycling races as well as for simulated ones. This last category of applications embeds digital simulation/management games AND board games played with figurines.

The cycling classification activities that are managed by **Cycling Rankings Library** relate to:
- Road races
- Track events
- Mountain Bike races
- Cross-Country races
- Gravel races
- BMX races
- BMX Indoor races


## 2. Interface between calling applications and the library

The **Cycling Rankings Library** specifies the classes, methods and functions that can be called by applications. Those software entities relates to:
- passing intermediate or daily rankings to the library (individuals and teams);
- asking for the modification of classifications at the calling applications will (intermediate, daily, general and final ones, or yearly challenges, etc.);
- passing the newly modified classifications back to the calling applications.

**Cycling Rankings Library** specifies also the data structures to be passed to it by the calling applications and the data structures returned back to them once classifications have been modified.


## 3. Programming Languages

First developments will be done in c++ with Visual Studio 2026. CMake files will then be generated to use gcc and clang under Linux.
Notice: c++20 will be the used standard at first (by Sep. 2026).

Python developments will be done under Visual Studio Code or under VS 2026 - still not specified.
Notice: Python 3.15 will be the used standard at first (by Oct. 2026).

Kotlin developments will be done under Android Studio.
Notice: Kotlin 2.4 will be the used standard at first (by Sep. 2026).

It may be that a PHP version of the library will be implemented with the help of Visual Studio Code.
PHP 8.4 or PHP 8.5 will be the used standard at first (by Sep. 2026, may change according to the date of the coding start, if any) .


## 4. Library components

The library is architectured as packages dedicated to the specific entities it manages. According to the similar activities and classification types that are used in the different cycling disciplines, some components are common to many of them.

So, some library components are grouped into package `commons` - e.g. riders identifiers, teams identifiers, teams compositions, sorting algorithms related to classifications based on points or times. Some other components are definitively specific to special activities and are then grouped into packages related to these special activities - e.g. comparison operators used for sorting and ranking, evaluation of times used for the ranking of teams (daily as well as general classifications), evaluation of the composition of heats for some Track events, etc.
