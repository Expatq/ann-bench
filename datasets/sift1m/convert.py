#!/usr/bin/env python3
"""Convert SIFT1M fvecs to headerless float32 little-endian matrices.

Generate dataset.json from the input shapes, preserve coordinate bytes and row
order, and remove data/raw/ after both matrices and metadata have been saved.
"""

import argparse
import json
import math
import shutil
import struct
import tempfile
from pathlib import Path


def Convert(sourcePath: Path, destinationPath: Path) -> tuple[int, int]:
    print(f"Converting {sourcePath.name}", flush=True)
    with sourcePath.open("rb") as source:
        header = source.read(4)
        if len(header) != 4:
            raise ValueError(f"{sourcePath}: missing fvecs dimension")
        dim = struct.unpack("<I", header)[0]
        if dim == 0:
            raise ValueError(f"{sourcePath}: dimension must be positive")

        recordBytes = 4 + dim * 4
        rows, remainder = divmod(sourcePath.stat().st_size, recordBytes)
        if remainder:
            raise ValueError(f"{sourcePath}: file size is not a multiple of {recordBytes} bytes")

        source.seek(0)
        with destinationPath.open("wb") as output:
            for row in range(rows):
                record = source.read(recordBytes)
                if len(record) != recordBytes or record[:4] != header:
                    raise ValueError(f"{sourcePath}: invalid fvecs record at row {row}; expected dim={dim}")
                coordinates = record[4:]
                if not all(math.isfinite(value) for (value,) in struct.iter_unpack("<f", coordinates)):
                    raise ValueError(f"{sourcePath}: non-finite coordinate at row {row}")
                output.write(coordinates)
    return rows, dim


def Main() -> None:
    argparse.ArgumentParser(description=__doc__).parse_args()
    directory = Path(__file__).resolve().parent
    dataDirectory = directory / "data"
    rawDirectory = dataDirectory / "raw"

    with tempfile.TemporaryDirectory(prefix=".convert-", dir=dataDirectory) as temporary:
        temporaryDirectory = Path(temporary)
        baseCount, dim = Convert(
            rawDirectory / "sift_base.fvecs", temporaryDirectory / "base.bin"
        )
        queryCount, queryDim = Convert(
            rawDirectory / "sift_query.fvecs", temporaryDirectory / "queries.bin"
        )
        if dim != queryDim:
            raise ValueError(f"base and query dimensions differ: {dim} and {queryDim}")

        config = {
            "metric": "l2",
            "dim": dim,
            "base_count": baseCount,
            "query_count": queryCount,
        }
        (temporaryDirectory / "dataset.json").write_text(
            json.dumps(config, indent=2) + "\n", encoding="utf-8"
        )

        for filename in ("base.bin", "queries.bin"):
            (temporaryDirectory / filename).replace(dataDirectory / filename)
        (temporaryDirectory / "dataset.json").replace(directory / "dataset.json")

    shutil.rmtree(rawDirectory)
    print(f"Vectors saved to {dataDirectory}; metadata saved to {directory / 'dataset.json'}")


if __name__ == "__main__":
    Main()
