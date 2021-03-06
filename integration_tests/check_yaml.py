#!/usr/bin/python3

""" Check results of execution of integration tests.

Compare .yaml files:
 - Generated by integration tests
 - Canonized correct results (reference data)
"""

import argparse
import yaml

"""
Compare yaml objects:
add possibility of specifying the correct value ranges in
expected object.

EXAMPLES:
// expected_file
some_key: [0, 3]  ===> OK
// observed_file
some_key: 2

// expected_file
some_key: [0, 3]  ===> :( assertion failed
// observed_file
some_key: 4
"""
def compareYaml(expected, observed):
    ex_type = type(expected)
    obs_type = type(observed)
    if ex_type == obs_type:
        if ex_type == list:
            assert len(expected) == len(observed)
            for (ex, ob) in zip(expected, observed):
                compareYaml(ex, ob)
        elif ex_type == dict:
            assert len(expected) == len(observed)
            for (key, value) in expected.items():
                assert key in observed
                compareYaml(value, observed[key])
        else:
            assert expected == observed
    else:
        assert ex_type == list and obs_type == int
        assert len(expected) == 2
        assert observed >= expected[0] and observed <= expected[1]

def compare(expected_filename, observed_filename):
    with open(expected_filename) as expected_file:
        expected = yaml.load(expected_file)
    with open(observed_filename) as observed_file:
        observed = yaml.load(observed_file)
    compareYaml(expected, observed)

def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    parser.add_argument(
        '--expected',
        type=str,
        help='Yaml file with expected results',
        required=True,
    )
    parser.add_argument(
        '--observed',
        type=str,
        help='Yaml file with observed results',
        required=True,
    )
    args = parser.parse_args()
    compare(args.expected, args.observed)

if __name__ == '__main__':
    main()
