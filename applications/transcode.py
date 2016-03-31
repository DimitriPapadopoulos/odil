import logging

import _odil

def add_subparser(subparsers):
    parser = subparsers.add_parser(
        "transcode",
        help="Change the transfer syntax or the format")
    parser.add_argument("input", help="Input file name")
    parser.add_argument("output", help="Output file name")
    parser.add_argument(
        "--format", "-f",
        choices=["binary", "json", "xml"], default="binary",
        help="Output file format")
    parser.add_argument(
        "--transfer-syntax", "-t",
        help="Transfer syntax of output file (binary only)")
    parser.add_argument(
        "--pretty-print", "-p", action="store_true",
        help="Pretty-print in output file (JSON and XML only)")
    parser.set_defaults(function=transcode)

def transcode(input, output, format, transfer_syntax, pretty_print):

    kwargs = {}

    if format == "binary":
        if transfer_syntax:
            transfer_syntax = getattr(_odil.registry, transfer_syntax)
            ts_info = _odil.registry.uids_dictionary[transfer_syntax]
            if ts_info.type != "Transfer Syntax":
                raise Exception(
                    "{} is not a transfer syntax".format(ts_info.name))
            kwargs["transfer_syntax"] = transfer_syntax
        else:
            kwargs["transfer_syntax"] = None
    elif format == "json":
        kwargs["pretty_print"] = pretty_print
    elif format == "xml":
        kwargs["pretty_print"] = pretty_print

    globals()["as_{}".format(format)](input, output, **kwargs)

def as_binary(input, output, transfer_syntax):
    _, data_set = _odil.read(input)
    _odil.write(data_set, output, transfer_syntax=transfer_syntax)

def as_json(input, output, pretty_print):
    _, data_set = _odil.read(input)
    with open(output, "w") as fd:
        json = _odil.as_json(data_set, pretty_print)
        fd.write(json)

def as_xml(input, output, pretty_print):
    _, data_set = _odil.read(input)
    with open(output, "w") as fd:
        xml = _odil.as_xml(data_set, pretty_print)
        fd.write(xml)
