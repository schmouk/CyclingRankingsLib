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

First, let's state what **Cycling Rankings Library** is not for: it does not prepare any display of rankings; it does not help for the management of data on disk or on the cloud either (in files or databases). This is left to the responsibility of the calling applications.

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

Notably, passed data is specified as ad-hoc structures as well as CSV format. Both are systematically available.


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

The architecture of the library is set as packages dedicated to the specific entities it manages. According to the similar activities and classification types that are used in the different cycling disciplines, some components are common to many of them.

So, some library components are grouped into package `commons` - e.g. riders identifiers, teams identifiers, teams compositions, sorting algorithms related to classifications based on points or times. Some other components are definitively specific to special activities and are then grouped into packages related to these special activities - e.g. comparison operators used for sorting and ranking, evaluation of times used for the ranking of teams (daily as well as general classifications), evaluation of the composition of heats for some Track events, etc.

Next subsections provide minimalist specifications of the main library components

### 4.1 Identifiers and Ranks

### 4.1.1 Identifiers
Riders and Teams are uniquely identified by integer numbers. **Cycling Rankings Library** does not manage names. Calling application provide numerical identifiers of their own. It is their responsibility to internally associate names and any other related information with these identifiers - that they may choose as they wish.

For some other classifications, identifiers for stages or for climbs may be needed. These identifiers are also integers. For stages, KOMS or intermediate sprint, these identifiers should get their values in ascending order along the courses of the overall race. It is the responsibility of the calling application to ensure these requirements.

### 4.1.2 Ranks
Ranks are coded in (signed) integers. Correct ranks start at 1 and are all increasing in ascending order except for a special case: ties. In case of ties, a same rank may be duplicated. For `r_t` the rank value of the tie and `n` the number of ties, n ranks get value `r_t` and the next one gets value `r_t + n`.  
Three special cases are also used and may be duplicated for a same ranking:
- Not Starting opponents' rank is denoted DNS (Did Not Start); DNS = 0;
- Not Finishing opponents' rank is denoted DNF (Did Not Finished); DNF = -1;
- Disqualified opponents' rank is denoted DSQ (DiSQualified); DSQ = -2.

Calling applications must ensure the correct use of these conventions. The library will apply then the correct algorithms and remove opponents when needed from the returned general classification.

### 4.2 Teams composition
When teams classification enters the game, whatever the underlying mode of classification (adding times, ranks or points), teams compositions are provided by the calling application as lists of riders identifiers or as CSV lines.

### 4.3 Time performance
When time comes to add time performances, for riders (general classification) or for teams (daily or general classification), times are internally represented as \<number of seconds, rational fraction of second>. A rational is a pair of values (numerator, denominator).

The library specifies a special type that helps representing times as \<hours, minutes, seconds, fraction>. The library implements functions for the transformation of this type to \<number of seconds, rational fraction of second> and vice versa.

Notice: for some board games, time performance is associated with the number of rounds for a figurine to cross the finish line. Rankings are then evaluated according to the difference btw. the min count of rounds over all figurines and the count for each figurine. Ties are broken according to the addition of ranks on stages.  
**Cycling Ranking Library** allow the association of a multiplicative coefficient on the differences. This multiplicative coefficient may vary per stage. The "time" performance is then the accumulated values \<coeff> x \<rounds diff> over stages. All values are then modified by substracting to all of them the value associated to the figurine ranking first in the newly modified classification.

### 4.4 Points performances
Points are positive values. They are associated with ranks and riders/teams identifiers.

### 4.5 Penalties
It may happen that time and/or points penalties are imposed to competitors (riders or teams). Time penalties are provided as positive values. Points penalties are set as negative values.

Notice: When a rider or a team gets a negative overall score, its identifier and its score are inserted in a general classification at the very end of the classification, ordered in descending order on the score values, and associated with a specific rank value: 0. This is a very specific case dedicated to general classifications. It has not to be confused with the DNS value for ranks as specified in subsection 4.1.2.

### 4.6 General classifications
**Cycling Rankings Library** does not store any general classification. For these to be modified by the library, calling application must provide the actual state of the general classification to be modified plus the rankings and performances associated with riders/teams identifiers for the actual modifying ranking. The calling application gets back the modified general classification.

The rankings for some general classifications may involve mandatory complementary information. For instance, the number of wins at the top of different climb categories plus the identifier of the last win for each category may be needed to break ties detected in the KOM new general classification. Those mandatory complementary information must be provided by the calling application.

The new ranking for a general classification is evaluated according to a sort algorithm that is directly related to the type of general classification. As such, many different sorting algorithms are provided by **Cycling Rankings Library**. See below

### 4.7 Ranking algorithms
The simplest ranking algorithms are the one dealing with just times or points. They are provided by the library.

Some more complex algorithms are also provided by the library, notably for breaking ties on specific classifications - e.g. breaking ties on time according to the associated fractional part of time, or according to the accumulation of ranks on every run stage.

All the sorting algorithms provided by **Cycling Ranking Library** are well specified and well documented. They cover ALL the known rules for ranking and breaking ties, for all cycling disciplines of Road, Track, Mountain bike, Cross-Country, Gravel, BMX, Indoor BMX.

It is the responsibility of the calling applications to specify the ranking algorithm to be used for a specific general classifications. This can be done at call time, when asking for the modification of a general classifications.

### 4.8 Asking for temporary classifications
This can be split in two parts. See the two subsections below.

#### 4.8.1 Complete state of new rankings
When time comes for the modification of a general classification according to an intermediate sprint (or KOM), the calling application provides the actual state of the general classification and the actual ranking for the intermediate sprint. It may then:
- either ask for the new state of the general classification, and provided it at the time of the next intermediate sprint for the same general classification;
- or ask the library to remember the actual state of the general classification, not to have to provide it back next time, while receiving nevertheless the current state of this general classification.

#### 4.8.2 Intermediate state of rankings
When time comes to evaluate teams performances by the accumulation of their riders performances, the global teams classification may be evaluated on the fly, evaluated on the teams performances that are fully evaluated and for which ranks are definitively set (i.e. they can't be modified by next performances of other teams).  
In this case, calling applications may ask for the definitive classification at every entry of next rider performance (next in rank) and get back the definitive top of the classification as soon as it is surely evaluated.

This is true also for challenges, when their new rankings may be evaluated on the fly.


### 4.9 Utilities
**Cycling Rankings Library** implements some useful utilities that are not immediately related to rankings. They are shortly listed in this section.

#### 4.9.1 Heats compositing
Some events lead to the organization of heats, repechage rounds, quarter of finals, semi-finals, finals and the like. This is especially true for Track or BMX events.

**Cycling Rankings Library** helps the evaluation of the composition of these heats and the like. This may be of help for true competitions organization. It will eventually help the coding of digital simulation games or the organization of board games.

These composition heats are set according to the known rules of competition. These rules may have evolved over time. They will eventually keep on evolving over time also. The implemented rules in **Cycling Rankings Library** are clearly documented. The associated algorithms are clearly documented also. The calling applications may specify a year value to get the specific form of the algorithm associated with this year for a same competition event type (e.g. Track Sprint event).

#### 4.9.2 Randomness
When time comes for randomness, for instance for compositing heats, or as an helper for digital simulations (see examples below) **Cycling Rankings Library** implements the *PCG XSH RS 64/32 (LCG)* algorithm (see reference [1] at end of this document).

The interface with the pseudo-random numbers generator is simple and well described. The calling applications may ask for random generation at any time. They may provide also a specific seed to repeat "random" series (for tests purpose). Finally, some specific applications may use it to simulate dices rolls, for the usage of board games for instance.

#### 4.9.3 Average speed
When providing a distance (in kilometers or in miles, may be with a fraction added to it) and a time with its precision (hh:mm, hh:mm:ss, hh:mm:ss.frc), a **Cycling Rankings Library** returns the related average speed in km/h or in mph with a fractional value precision adapted to the precision of the provided time. The precision of the fractional part of the average speed contains up to three digits and the number of digits (0, 1, 2 or 3) is adapted to the precision of the provided time: the better the time rpecision, the better the average speed precision.  
The returned value is a string.


## 5. References

[1] *PCG: A Family of Simple Fast Space-Efficient Statistically Good Algorithms for Random Number Generation.* 
Submitted to ACM Transactions on Mathematical Software (47 pages)  
Finally: Harvey Mudd College Computer Science Department Technical Report, HMC-CS-2014-0905, Issued: September 5, 2014 (56 pages).  
@techreport{oneill:pcg2014, title = "PCG: A Family of Simple Fast Space-Efficient Statistically Good Algorithms for Random Number Generation", author = "Melissa E. O'Neill", institution = "Harvey Mudd College", address = "Claremont, CA", number = "HMC-CS-2014-0905", year = "2014", month = Sep, xurl = "https://www.cs.hmc.edu/tr/hmc-cs-2014-0905.pdf", }  
see also https://www.pcg-random.org/pdf/hmc-cs-2014-0905.pdf.
