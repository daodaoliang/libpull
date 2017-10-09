#!/bin/sh -e

# Fetch external dependencies
if [ -d "ext" ]; then
    rm -rf ext
fi
mkdir ext

# Clone the contiki repository and its submodules
echo "Cloning the Contiki-NG repository..."
git clone --quiet --progress \
    https://github.com/contiki-ng/contiki-ng.git ext/contiki-ng
(cd ext/contiki-ng
git checkout  51f5073
git submodule update --init --recursive
)
echo "Cloning the Contiki-NG repository...done"

# Clone and build tinydtls
echo "Cloning tinydtls..."
git clone --quiet --progress --recurse-submodules \
    --single-branch --depth 2 https://git.eclipse.org/r/tinydtls/org.eclipse.tinydtls \
    ext/tinydtls
echo "Cloning tinydtls...done"

# Patch the repositories
PATCHDIR=patches
for dir in $(cd $PATCHDIR && find * -type d -print); do
    for f in $(find $PATCHDIR/$dir -maxdepth 1 -name '*.patch' -print | sort); do
        patch=$PWD/$f
        echo "Applying patch: $patch"
        (cd ext/$dir/
        git am $patch
        )
    done
done
