#!/usr/bin/env python3

"""
Replace comma-separated Taxonomy IDs with the label TRUE, FALSE, or NULL:
TRUE if any of the IDs is in the input set, FALSE if none is, and NULL if there are no IDs.
"""
from argparse import ArgumentParser
import logging
import os
import sys

__author__ = 'Florian Leitner'
__version__ = '1.0'


def relabel(istream, tax_ids):
    for line in istream:
        line = line.strip('\n')
        items = line.split('\t')
        taxa_str = items.pop()
        print('\t'.join(items), end='')

        if taxa_str:
            logging.debug('tax IDs %s', taxa_str)

            if any(int(tid) in tax_ids for tid in taxa_str.split(',')):
                print('\tTRUE')
            else:
                print('\tFALSE')
        else:
            print('\tNULL')


epilog = 'system (default) encoding: {}'.format(sys.getdefaultencoding())
parser = ArgumentParser(
    usage='%(prog)s [options] LIST [FILE ...]',
    description=__doc__, epilog=epilog,
    prog=os.path.basename(sys.argv[0])
)

parser.add_argument('taxa', metavar='LIST', type=open,
                    help='a list of Taxononmy IDs (one ID per line)')
parser.add_argument('files', metavar='FILE', nargs='*', type=open,
                    help='input file(s); if absent, read from <STDIN>')
parser.add_argument('--version', action='version', version=__version__)
parser.add_argument('--verbose', '-v', action='count', default=0,
                    help='increase log level [WARN]')
parser.add_argument('--quiet', '-q', action='count', default=0,
                    help='decrease log level [WARN]')
parser.add_argument('--logfile', metavar='FILE',
                    help='log to file instead of <STDERR>')

args = parser.parse_args()
files = args.files if args.files else [sys.stdin]

# Logging setup
log_adjust = max(min(args.quiet - args.verbose, 2), -2) * 10
logging.basicConfig(filename=args.logfile, level=logging.WARNING + log_adjust,
                    format='%(levelname)-8s %(module) 10s: %(funcName)s %(message)s')
logging.info('verbosity increased')
logging.debug('verbosity increased')

tax_ids = {int(tid) for tid in args.taxa}
logging.info('loaded %s Taxonomy IDs', len(tax_ids))
logging.debug('TaxIDs: %s', tax_ids)

for input_stream in files:
    try:
        relabel(input_stream, tax_ids)
    except:
        logging.exception("unexpected program error")
        parser.error("unexpected program error")
