#!/bin/bash
# create multiresolution windows icon
ICON_SRC=../../src/qt/res/icons/trustycoin.png
ICON_DST=../../src/qt/res/icons/trustycoin.ico
convert ${ICON_SRC} -resize 16x16 trustycoin-16.png
convert ${ICON_SRC} -resize 32x32 trustycoin-32.png
convert ${ICON_SRC} -resize 48x48 trustycoin-48.png
convert trustycoin-16.png trustycoin-32.png trustycoin-48.png ${ICON_DST}

