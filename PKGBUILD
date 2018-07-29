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
url='https://discordapp.com/'
license=('MIT')
depends=(
'gcc' 
'yaml-cpp'
)
source=("git://github.com/triploit/tridymite.git")
sha256sums=('SKIP')

package() {
  #Create directory for make
  mkdir "${srcdir}/tridymite/obj"
  
  #Make tridymite
  make -C "${srcdir}/tridymite"
  
  # Install the main binary.
  install -d "${pkgdir}/opt/${pkgname}"
  cp -a "${srcdir}/tridymite/tridy" "${pkgdir}/opt/${pkgname}/tridy"

  # Exec bit
  chmod 755 "${pkgdir}/opt/${pkgname}/tridy"

  # Link binary
  mkdir -p ${pkgdir}/usr/bin
  ln -s "/opt/${pkgname}/tridy" "${pkgdir}/usr/bin/tridy"
  
  # License
  install -Dm644 "${srcdir}/tridymite/LICENSE" "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"
}
