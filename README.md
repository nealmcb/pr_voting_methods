# pr_voting_methods presents Proportional Representation Voting Methods and data

[Proportional Representation](https://en.wikipedia.org/wiki/Proportional_representation)
is the goal of a variety of voting methods, as described in a 2017 presentation
[Proportional Representation for LWVBC](http://bcn.boulder.co.us/~neal/elections/proportional-representation-lwv/#1)

We present here code to implement a variety of proportional representation voting methods, and some data on how individual ballots have been cast in some multi-winner contests, and analyze how different election methods would handle the same votes.

This perhaps makes most sense when working with a variety of voting methods which use the same ballots used for plurality voting contests, and for [approval voting](https://en.wikipedia.org/wiki/Approval_voting).

The Block Plurality Voting method is commonly used in council and board of election contests in the US. In an election with N winners, it allows voters to vote for up to N candidates.  So, with the exception that we don't see how the voters would vote if they could vote for more than N candidates, this can give some insight into methods like Proportional Approval Voting.

## Currently implemented:

* [Proportional Approval Voting](https://en.wikipedia.org/wiki/Proportional_approval_voting)

## Reported results

* City of Westminster Colorado City Councilor (Vote for 3)
 * Results in 2015, when there were 10 candidates
  * [Adams County](http://results.enr.clarityelections.com/CO/Adams/56803/157259/Web01/en/summary.html)
  * [Jefferson County](http://results.enr.clarityelections.com/CO/Jefferson/56801/157251/Web01/en/summary.html)
 * Cast Vote Record Data from 2015
  * westminster_adams.cvr.csv
