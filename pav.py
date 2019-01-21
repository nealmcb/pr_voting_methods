#!/usr/bin/env python3
"""pav: Tally Proportional Approval Voting CVRs.

Examples:

    ./pav.py Byers_SD_32J_Adams.csv 4

    ./pav.py --help

SPDX-License-Identifier:	MIT

TODO:
  build on uniq = collections.Counter(cvrs) to replace further usage of cvrs

  refactor with clean parsing, cvr representation, comparison to plurality, etc so it fits in with hypothesis testing which can be generalized to SPAV, STV etc.

  show percent gains for utility of optimal vs plurality
  allow grading utility of arbitrary slates of winners
  make optimization optional

"""

import os
import sys
import logging
import argparse
import collections
import itertools

__author__ = "Neal McBurnett <http://neal.mcburnett.org/>"
__version__ = "0.3.0"
__date__ = "2017-06-28"
__copyright__ = "Copyright (c) 2017 Neal McBurnett"

parser = argparse.ArgumentParser(description='pav: Tally Proportional Approval Voting CVRs.')

parser.add_argument('cvrfile', nargs='?',
                    help='CVR file: header for candidates, and 0 or 1 for each candidate (or leave them all blank)')

parser.add_argument("num_winners", type=int, nargs='?',
  help="Number of winners")

parser.add_argument('-v', '--version', action='store_true',
                    help='Print version number, or print verbose test results')

parser.add_argument("--test",  action="store_true", default=False,
  help="Run tests")

parser.add_argument("-d", "--debuglevel", type=int, default=logging.WARNING,
  help="Set logging level to debuglevel, expressed as an integer: "
  "DEBUG=10, INFO=20, endpoint timing=25, WARNING=30, ERROR=40, CRITICAL=50. "
  "The default is %(default)s" )


def harmonic(matches):
    """Calculate harmonic series sum up to the integer 'matches'
    >>> harmonic(0)
    0.0
    >>> harmonic(2)
    1.5
    """
    utility = 0.0
    for i in range(1, matches + 1):
        utility += 1 / i

    return utility


UTILITY = [harmonic(i) for i in range(0, 100)]


def utility(winnerset, cvr):
    """Return utility to voter who voted cvr of given winnerset.
    The winnerset should be a valid set of winners in a PAV election.
    The cvr should be a valid set of candidates voted for.

    >>> utility(frozenset(["c1", "c2", "c4"]), frozenset(["c1", "c3", "c4"]))
    1.5
    """

    # u = 
    # logging.debug("perm %s utility %.2f cvr %s len %d inters %s" % (winnerset, u, cvr, len(winnerset.intersection(cvr)), winnerset.intersection(cvr)))
    return UTILITY[len(winnerset.intersection(cvr))]


def bools_to_set(tuple):
    "Return a frozenset of the field names for all values in the given namedtuple which are 1, not 0 or NaN"

    return frozenset(key for key, value in tuple._asdict().items() if value > 0)


def res_to_str(res):
    return ("%.3f" % res[1], sorted(list(res[0])))


def read_cvrs(filename, num_winners):
    """Read CVR files in CSV format from filename and return list of BCVRs.

    CSV file should have one column per candidate, identified in headers,
    and have either a "0" or a "1" for each candidate in each row.
    """

    import pandas as pd

    df = pd.read_csv(filename)

    df.dropna(inplace=True)

    plurality = df.sum()

    plurality.sort_values(inplace=True, ascending=False)
    print("Plurality results in order")
    print(plurality)
    print()

    plurality_winners = list(plurality.index[:num_winners])

    # all_candidates = type(next(df.itertuples(False)))._fields
    tuples = df.itertuples(False)
    return tuples, df


def tally_pav_for_result(winnerset, cvrs, num_winners):
    """Tally the given cvrs based on the given result, assuming num_winners.
    return the score
    """

    return sum(utility(winnerset, cvr) for cvr in cvrs)


def tally_pav(binary_cvrs, num_winners):
    """Tally list of BCVRs with Proportional Approval Voting method

    Test: reproduce the example from https://en.wikipedia.org/wiki/Proportional_approval_voting
    FIXME: don't fail if it produces different order of elements in frozensets (cpython vs pypy)
    >>> BCVR = collections.namedtuple('BCVR', ['A', 'B', 'C', 'D'])
    >>> binary_cvrs = [BCVR(1, 1, 0, 0)] * 5 + [BCVR(1, 0, 1, 0)] * 17 + [BCVR(0, 0, 0, 1)] * 8
    >>> winner, cvrs, scores = tally_pav(binary_cvrs, 2)
    30 CVRs, 3 unique selections of candidates
    <BLANKLINE>
    Proportional Approval Voting results
    <BLANKLINE>
    Top 10 scores:
    ('30.500', ['A', 'C'])
    ('30.000', ['A', 'D'])
    ('25.000', ['C', 'D'])
    ('24.500', ['A', 'B'])
    ('22.000', ['B', 'C'])
    ('13.000', ['B', 'D'])
    <BLANKLINE>
    Max score ('30.500', ['A', 'C'])
    """

    cvrs = []
 
    for cvr in binary_cvrs:
        cvrs.append(bools_to_set(cvr))

    all_candidates = type(cvr)._fields
    uniq = collections.Counter(cvrs)
    print("{} CVRs, {} unique selections of candidates".format(len(cvrs), len(uniq)))

    # plurality_utility = sum(utility(frozenset(plurality_winners), cvr) for cvr in cvrs)
    # print("Score is %.3f for plurality winners %s" % (plurality_utility, plurality_winners))

    possible_results = itertools.combinations(all_candidates, num_winners)

    scores = {}
    for result in possible_results:
        resultset = frozenset(result)
        scores[resultset] = tally_pav_for_result(resultset, binary_cvrs, num_winners)

    print("\nProportional Approval Voting results")
    print("\nTop 10 scores:")
    [print(res_to_str(res)) for res in sorted(scores.items(), key=lambda tuple: tuple[1], reverse=True)[:10]]

    winner = max(scores.items(), key=lambda tuple: tuple[1])
    print("\nMax score {}".format(res_to_str(winner)))

    return winner, cvrs, scores


def _test():
    import doctest
    return doctest.testmod()


def _timing():
    "Flexible test for timing.  Choose any legal values for candidates (52 or below) and winners"

    candidates = 10
    winners = 5
    BCVR = collections.namedtuple('BCVR', " ".join("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")[0:candidates*2])  #list(range(candidates)
    b = [1] + [0] * (candidates -1)
    winner, cvrs, scores, df = tally_pav([BCVR(*b)] * 100, winners)
    print(winner)

    bcvrs, df = read_csv("Westminster_Adams.csv")
    winner, cvrs, scores, df = tally_pav(bcvrs, 3)
    print(winner)


def main(parser):
    "Run pav with given argparse arguments"

    args = parser.parse_args()

    logging.basicConfig(level=args.debuglevel)

    logging.debug("args: %s", args)

    if args.test:
        _test()
        sys.exit(0)

    elif args.version:
        print("%s version %s" % (parser.prog, __version__))
        sys.exit(0)

    elif args.cvrfile and args.num_winners:
        bcvrs, df = read_cvrs("Westminster_Adams.csv", args.num_winners)
        winner, cvrs, scores, df = tally_pav(args.cvrfile, args.num_winners)

    else:
        parser.print_help(sys.stderr)


if __name__ == "__main__":
    main(parser)
