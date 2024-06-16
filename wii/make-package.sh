#! /bin/sh

set -e

NAME=gl-117
TMPDIR="$(mktemp -d)"
OUTDIR="$TMPDIR/$NAME"

mkdir -p "$OUTDIR"
cp -a \
    textures/ \
    sounds/ \
    music/ \
    models/ \
    wii/meta.xml \
    wii/icon.png \
    "$OUTDIR"
mkdir -p "$OUTDIR/.gl-117"
cp wii/conf* "$OUTDIR/.gl-117/"
elf2dol "src/$NAME.elf" "$OUTDIR/boot.dol"

cd $TMPDIR
zip -r "$NAME.zip" "$NAME"
cd -
mv "$TMPDIR/$NAME.zip" .
rm -rf "$TMPDIR"
