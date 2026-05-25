#!/usr/bin/env python3

from __future__ import annotations

import argparse
import shutil
import sys
from pathlib import Path


REPO_ROOT = Path(__file__).resolve().parent
TARGET_DIR = REPO_ROOT / "vvenc" / "source" / "Lib" / "CommonLib"

VARIANT_FILES = {
    "baseline": ("MCTF.cpp",),
    "evolved-slower": ("MCTF.cpp", "BimHeuristic.h", "BimHeuristicGenerated.h"),
    "evolved-faster": ("MCTF.cpp", "BimHeuristic.h", "BimHeuristicGenerated.h"),
}

BASELINE_REMOVE_FILES = ("BimHeuristic.h", "BimHeuristicGenerated.h")


def remove_file_if_exists(path: Path, dry_run: bool) -> None:
    if not path.exists():
        print(f"skip remove: {path.name} (not present)")
        return

    print(f"remove: {path.name}")
    if not dry_run:
        path.unlink()


def copy_file(src: Path, dst: Path, dry_run: bool) -> None:
    if not src.is_file():
        raise FileNotFoundError(f"missing source file: {src}")

    print(f"copy: {src.relative_to(REPO_ROOT)} -> {dst.relative_to(REPO_ROOT)}")
    if not dry_run:
        shutil.copy2(src, dst)


def switch_variant(variant: str, dry_run: bool) -> None:
    if variant not in VARIANT_FILES:
        raise ValueError(f"unsupported variant: {variant}")

    source_dir = REPO_ROOT / variant
    if not source_dir.is_dir():
        raise FileNotFoundError(f"missing variant directory: {source_dir}")

    if not TARGET_DIR.is_dir():
        raise FileNotFoundError(f"missing target directory: {TARGET_DIR}")

    if variant == "baseline":
        for filename in BASELINE_REMOVE_FILES:
            remove_file_if_exists(TARGET_DIR / filename, dry_run)

    for filename in VARIANT_FILES[variant]:
        copy_file(source_dir / filename, TARGET_DIR / filename, dry_run)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Switch vvenc/source/Lib/CommonLib to baseline, evolved-slower, or evolved-faster files."
    )
    parser.add_argument(
        "variant",
        choices=sorted(VARIANT_FILES),
        help="Variant to copy into vvenc/source/Lib/CommonLib.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Show the planned file operations without modifying files.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()

    try:
        switch_variant(args.variant, args.dry_run)
    except Exception as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    print(f"done: {args.variant}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
