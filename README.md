# Proportional Representation Voting Methods: Code, Data, and Auditing

[Proportional Representation](https://en.wikipedia.org/wiki/Proportional_representation)
is the goal of a variety of voting methods, as described e.g. in a 2017 presentation
[Proportional Representation for the League of Women Voters, Boulder County (LWVBC)](http://bcn.boulder.co.us/~neal/elections/proportional-representation-lwv/#1)
and in a 2019 presentation on *Multi-Winner Approval Voting*, available at
[Voting Methods Team - League of Women Voters of Boulder County](https://www.lwvbc.org/content.aspx?page_id=22&club_id=629866&module_id=301984).

The `pr_voting_methods` project provides code to implement a variety of proportional representation voting methods, and some data on how individual ballots have been cast in some multi-winner contests, and analyzes how different election methods would handle the same votes.

Proportional representation is perhaps easiest to implement in the US when working with voting methods which use the same ballots used for plurality voting contests, and for [approval voting](https://en.wikipedia.org/wiki/Approval_voting).

The [Plurality Block Voting](https://en.wikipedia.org/wiki/Plurality-at-large_voting) method is commonly used in council and board of election contests in the US. In an election with N winners, it allows voters to vote for up to N candidates.  So, with the important exception that we don't see how the voters would vote if they could vote for more than N candidates, we can tabulate them using voting methods that use an approval voting ballot, and get at least some insight into methods like Proportional Approval Voting.

## Try it out for free!
You can click [![Binder](https://mybinder.org/badge.svg)](https://mybinder.org/v2/gh/nealmcb/pr_voting_methods/master)
to launch a free online notebook, via the [Binder](https://mybinder.org/) service, in which you can run this code (e.g. the `pav.ipynb` notebook) online.

On a notebook on Binder, you can use `File/Open/Upload` to upload your own Cast Vote Records in csv format (like the canned example in [Byers_SD_32J_Adams.csv](Byers_SD_32J_Adams.csv) and analyze them in the same way.

## Currently implemented, in Python 3:

* pav.py: [Proportional Approval Voting](https://en.wikipedia.org/wiki/Proportional_approval_voting)

## Reported results

* City of Westminster Colorado City Councilor (Vote for 3)
  * Cast Vote Record data on each ballot from 2015, when there were 10 candidates
    * Westminster_Adams.csv
    * *Westminster results for Jefferson county: should be available soon*....

* Byers School District (Vote for 4)
  * Cast Vote Record data on each ballot from 2015, when there were 8 candidates
    * Byers_SD_32J_Adams.csv
    * We have no Cast Vote Records for the Arapahoe County portion of this contest for 2015

* Official Results by county in 2015
  * [Adams County](http://results.enr.clarityelections.com/CO/Adams/56803/157259/Web01/en/summary.html)
  * [Jefferson County](http://results.enr.clarityelections.com/CO/Jefferson/56801/157251/Web01/en/summary.html)

## Proportional Approval Voting results

See the [PAV notebook](https://github.com/nealmcb/pr_voting_methods/blob/master/pav.ipynb)
for PAV results, so far with just the Adams County results
in the 2015 Westminster Colorado City Council contest and Byers SD contest.

To run tests on the pav module (where no output means it is working):

    python3 pav.py --test

## Auditing Proportional Representation methods

Work relating to audits of PAV contests is in progress, and suggests that a
risk-limiting audit method would not be difficult to implement. That is also
true for Satisfaction Approval Voting (SAV).

Sequential Proportional Approval Voting (SPAV) is likely to be trickier because it proceeds in multiple rounds.
See related work at https://people.eng.unimelb.edu.au/michelleb/IRV-auditing.pdf and
https://www.ece.rutgers.edu/~asarwate/pdfs/SarwateCS13irv.pdf .

Note that any voting method can be scientifically audited via Bayesian methods.

## Proportional Representation Outcomes Compared to Single-Winner Outcomes

Since there are more possible outcomes to distinguish, margins in general will
be tighter for multi-winner methods than for single-winner methods.

But if changing a small number of ballots would alter just one winner in a 5-winner
contest, it might not affect the balance of power or the legislative
decision-making much.  So the impact might be considered far less than with a
single-winner contest in a highly-polarized electorate.

It would also be interesting to look at the outcomes in other ways, e.g. the
risk that each individual candidate is or is not actually a winner, or the risk
that the "utility" of the outcome for the electorate (as defined e.g. by the PAV
score) is off by some fraction.

## TODO

Add more statistics.  E.g. for a PAV score, show it also as an average score per ballot, or per non-blank ballot, or show as a fraction of the maximum possible score.

## License
This project is licensed under the terms of the MIT license.

# See also

## Fast C-language implementation of Proportional Approval Voting by Warren Smith

Warren D. Smith has an extensive page on Proportional Approval Voting.
[RangeVoting.org - Optimal proportional representation](https://rangevoting.org/QualityMulti.html)

Fast C language code for the algorithms discussed in that paper is available at
[CleanOptPRVote.c](https://rangevoting.org/CleanOptPRVote.c)
and runs about 100 times faster than this Python code.
Besides being written in C, it also uses Knuth's clever "revolving-door" algorithm R.
as described on page 9 of his [pre-fascicle 3A, generating all combinations](https://www.kcats.org/csci/464/doc/knuth/fascicles/fasc3a.pdf).
It has the advangage of changing only one element of the winner set at each step,
so we can track and update the number of winners for the next ballot in constant time.
See also the "MC" algorithm at [Combinations](http://combos.org/comb).

An updated version of the code is here in the `harmonic` subdirectory.
Hopefully we can get it working as a library so it can be called from Python, to
make it easy to tally arbitrary elections more quickly.

## Auditing Open List Proportional Representation
The first risk-limiting audit result for proportional representation is published at
[Verifiable European Elections: Risk-limiting Audits
for D’Hondt and its relatives](https://www.usenix.org/jets/issues/0301/stark) by
Philip B. Stark and Vanessa Teague, March 26, 2015

The code to both allocate seats, and audit the allocation, is at
[DKDHondt14: IPython notebook for Risk-Limiting Audit of 2014 Danish portion of
the EU Parliamentary elections](https://github.com/pbstark/DKDHondt14)

It is demonstrated on [Denmark's 2014 European Union Parliamentary election](https://en.wikipedia.org/wiki/2014_European_Parliament_election_in_Denmark)
which uses an [open list](https://en.wikipedia.org/wiki/Open_list)
[proportional representation](https://en.wikipedia.org/wiki/Party-list_proportional_representation)
voting method, with seats allocated via the
[highest averages method](https://en.wikipedia.org/wiki/Highest_averages_method).

Note that in Denmark, parties can form coalitions, in which case first seats are
allocated across the coalitions, and then from the the seats for each coalition,
the parties within the coalition are allocated seats.
