#!/usr/bin/env python

import collections
import itertools

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


def utility(permutation, cvr):
    """Return utility to voter who voted cvr of given permutation
    >>> utility(frozenset(["c1", "c2", "c4"]), frozenset(["c1", "c3", "c4"]))
    1.5
    """
    return UTILITY[len(permutation.intersection(cvr))]


def bools_to_set(tuple):
    return frozenset(key for key, value in tuple._asdict().items() if value)


def tally_pav(binary_cvrs, num_winners):
    """Tally given csv file with Proportional Approval Voting method
    File should have one column per candidate, identified in headers,
    and have either a "0" or a "1" for each candidate in each row.

    Test: reproduce the example from https://en.wikipedia.org/wiki/Proportional_approval_voting
    FIXME: don't fail if it produces different order of elements in frozensets (cpython vs pypy)
    >>> BCVR = collections.namedtuple('BCVR', ['A', 'B', 'C', 'D'])
    >>> binary_cvrs = [BCVR(1, 1, 0, 0)] * 5 + [BCVR(1, 0, 1, 0)] * 17 + [BCVR(0, 0, 0, 1)] * 8
    >>> winner, cvrs, scores, df = tally_pav(binary_cvrs, 2)
    30 CVRs, 3 unique selections of candidates
    <BLANKLINE>
    Proportional Approval Voting results
    <BLANKLINE>
    Top 10 scores:
    (frozenset({'C', 'A'}), 30.5)
    (frozenset({'D', 'A'}), 30.0)
    (frozenset({'D', 'C'}), 25.0)
    (frozenset({'A', 'B'}), 24.5)
    (frozenset({'C', 'B'}), 22.0)
    (frozenset({'D', 'B'}), 13.0)
    <BLANKLINE>
    Max score: 30.5 for (frozenset({'C', 'A'}), 30.5)
    """

    if isinstance(binary_cvrs, str):
        import pandas as pd

        df = pd.read_csv(binary_cvrs)

        plurality = df.sum()
        plurality.sort_values(inplace=True, ascending=False)
        print("Plurality results in order")
        print(plurality)
        print()

        all_candidates = type(next(df.itertuples(False)))._fields
        tuples = df.itertuples(False)
    else:
        tuples = binary_cvrs
        df = None

    cvrs = []
 
    for cvr in tuples:
        cvrs.append(bools_to_set(cvr))

    all_candidates = type(cvr)._fields
    uniq = collections.Counter(cvrs)
    print("{} CVRs, {} unique selections of candidates".format(len(cvrs), len(uniq)))

    possible_results = itertools.combinations(all_candidates, num_winners)

    scores = {}
    for result in possible_results:
        resultset = frozenset(result)
        scores[resultset] = sum(utility(resultset, cvr) for cvr in cvrs)

    print("\nProportional Approval Voting results")
    print("\nTop 10 scores:")
    [print(res) for res in sorted(scores.items(), key=lambda tuple: tuple[1], reverse=True)[:10]]

    winner = max(scores.items(), key=lambda tuple: tuple[1])
    print("\nMax score: {} for {}".format(scores[winner[0]], winner))

    return winner, cvrs, scores, df

if __name__ == "__main__":
    import doctest
    doctest.testmod()

    # Flexible test for timing.  Choose any legal values for candidates (52 or below) and winners
    candidates = 10
    winners = 5
    BCVR = collections.namedtuple('BCVR', " ".join("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")[0:candidates*2])  #list(range(candidates)
    b = [1] + [0] * (candidates -1)
    winner, cvrs, scores, df = tally_pav([BCVR(*b)] * 100, winners)
    print(winner)

    winner, cvrs, scores, df = tally_pav("Westminster_Adams.csv", 3)
    print(winner)
