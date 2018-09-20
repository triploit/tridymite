# Maintainer: David Keller <blobcodes@gmail.com>

pkgname=tridymite-git
pkgver=r12.73626ab

pkgver() {
  cd "tridymite"
  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

pkgrel=1
pkgdesc="Tridymite - The Git Repo Package Manager"
arch=('x86_64')
url='https://github.com/triploit/tridymite'
license=('MIT')

depends=(
'gcc' 
'yaml-cpp'
)

source=("git://github.com/triploit/tridymite.git")
sha256sums=('SKIP')

package() {
  # Create directory for make
  mkdir "${srcdir}/tridymite/obj"
  
  # Make tridymite
  make -C "${srcdir}/tridymite"
  eval "${srcdir}/tridymite/obj/tridy local ${srcdir}/tridymite"
}
