#!/usr/bin/env python3
"""Download SIFT1M into data/raw/ next to this script; reuse an existing archive."""

import argparse
import shutil
import tarfile
import tempfile
from pathlib import Path
from urllib.request import urlopen

SOURCE_URL = "ftp://ftp.irisa.fr/local/texmex/corpus/sift.tar.gz"
FILENAMES = ("sift_base.fvecs", "sift_query.fvecs")


def Main() -> None:
    argparse.ArgumentParser(description=__doc__).parse_args()
    rawDirectory = Path(__file__).resolve().parent / "data" / "raw"
    rawDirectory.mkdir(parents=True, exist_ok=True)
    archivePath = rawDirectory / "sift.tar.gz"

    with tempfile.TemporaryDirectory(
        prefix=".download-", dir=rawDirectory
    ) as temporary:
        temporaryDirectory = Path(temporary)
        sourceArchive = archivePath
        if not archivePath.exists():
            print(f"Downloading {SOURCE_URL}", flush=True)
            sourceArchive = temporaryDirectory / archivePath.name
            with urlopen(SOURCE_URL, timeout=60) as source, sourceArchive.open(
                "wb"
            ) as output:
                shutil.copyfileobj(source, output, length=1024 * 1024)

        print(f"Extracting base and queries from {sourceArchive}", flush=True)
        with tarfile.open(sourceArchive, "r:gz") as archive:
            for filename in FILENAMES:
                member = archive.getmember(f"sift/{filename}")
                if not member.isfile():
                    raise ValueError(f"{member.name}: expected a regular file")
                with archive.extractfile(member) as source:
                    with (temporaryDirectory / filename).open("wb") as output:
                        shutil.copyfileobj(source, output, length=1024 * 1024)

        if sourceArchive != archivePath:
            sourceArchive.replace(archivePath)
        for filename in FILENAMES:
            (temporaryDirectory / filename).replace(rawDirectory / filename)
        print(f"Raw vectors saved to {rawDirectory}")


if __name__ == "__main__":
    Main()
