# Proportional Representation Voting Methods and Data

[Proportional Representation](https://en.wikipedia.org/wiki/Proportional_representation)
is the goal of a variety of voting methods, as described e.g. in a 2017 presentation
[Proportional Representation for the League of Women Voters, Boulder County (LWVBC)](http://bcn.boulder.co.us/~neal/elections/proportional-representation-lwv/#1)

The `pr_voting_methods` project provides code to implement a variety of proportional representation voting methods, and some data on how individual ballots have been cast in some multi-winner contests, and analyzes how different election methods would handle the same votes.

This perhaps makes most sense when working with a variety of voting methods which use the same ballots used for plurality voting contests, and for [approval voting](https://en.wikipedia.org/wiki/Approval_voting).

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

## TODO

Add more statistics.  E.g. for a PAV score, show it also as an average score per ballot, or per non-blank ballot, or show as a fraction of the maximum possible score.

## License
This project is licensed under the terms of the MIT license.
