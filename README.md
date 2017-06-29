# Proportional Representation Voting Methods and Data

[Proportional Representation](https://en.wikipedia.org/wiki/Proportional_representation)
is the goal of a variety of voting methods, as described e.g. in a 2017 presentation
[Proportional Representation for LWVBC](http://bcn.boulder.co.us/~neal/elections/proportional-representation-lwv/#1)

The pr_voting_methods project provides code to implement a variety of proportional representation voting methods, and some data on how individual ballots have been cast in some multi-winner contests, and analyzes how different election methods would handle the same votes.

This perhaps makes most sense when working with a variety of voting methods which use the same ballots used for plurality voting contests, and for [approval voting](https://en.wikipedia.org/wiki/Approval_voting).

The [Plurality Block Voting](https://en.wikipedia.org/wiki/Plurality-at-large_voting) method is commonly used in council and board of election contests in the US. In an election with N winners, it allows voters to vote for up to N candidates.  So, with the important exception that we don't see how the voters would vote if they could vote for more than N candidates, we can tabulate them using voting methods that use an approval voting ballot, and get at least some insight into methods like Proportional Approval Voting.

## Currently implemented:

* [Proportional Approval Voting](https://en.wikipedia.org/wiki/Proportional_approval_voting)

## Reported results

* City of Westminster Colorado City Councilor (Vote for 3)
 * Results in 2015, when there were 10 candidates
  * [Adams County](http://results.enr.clarityelections.com/CO/Adams/56803/157259/Web01/en/summary.html)
  * [Jefferson County](http://results.enr.clarityelections.com/CO/Jefferson/56801/157251/Web01/en/summary.html)
 * Cast Vote Record Data from 2015
  * westminster_adams.cvr.csv

## Proportional Approval Voting results

See the [PAV notebook](https://github.com/nealmcb/pr_voting_methods/blob/master/pav.ipynb) for PAV results for just the Adams County results in the  2015 Westminster Colorado City Council election.

## License
This project is licensed under the terms of the MIT license.
