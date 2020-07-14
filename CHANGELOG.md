# Changelog

Note: Date format is `DD.MM.YYYY` (01.02.2000 is 1'st February 2000). All 
versions listed here are possible to install using Tridymite (for example 
`tridy -i triploit:tridy#v1.0.4-beta.1`).

Note from 12.07.2020: The versioning of Tridymite is pretty messed up, because
at the start of the project I didn't really think about versioning and just gave
everything some increasing number-letter-dots-combination. I'm trying to improve 
this from now on.

### v1.0.7 (14.07.2020)

* fixed coloring at removing/updating messages
* added checking for conflicting versions in package dependencies
* added verbose argument

### v1.0.6 (12.07.2020)

* added statusbar for package-downloads
* added changelog

### v1.0.5-beta.1 (20.04.2020)

* fixed performance for GPPS packages
* fixed bugs in GPPS installations
* fixed sentences to translation fallback

### v1.0.4-beta.2 (23.02.2020)

* fixed translations
* added support for Tridymite's GPPS (Given Path Package Support)
    * installation of package-source into a given directory, with basic
    functionality like install, remove, update

### v1.0.4-beta.1 (08.06.2019)

* added support for Git tags

### v1.0.3-rc.5 (08.06.2019)

* added new versioning system
* added threading to download packages simultaneously

### v1.0.3b (03.06.2019)

* fixed minor bugs

### v1.0.3a (03.06.2019)

Note: There is a version v1.0.3 in the commits, which was a wrong version number.
It was changed to v1.0.3a.

* fixed yaml-cpp dependency in installation script

### v1.0.2f (03.06.2019)

Note: There was a v1.0.2e somewhere in the commit's, but it seamlessly merged 
(a metaphorical, no Git-merge) into v1.0.2f, so I will combine both here. (See
my second note about how I messed up the versioning.)

* added dependency-checking to installation script
* added argument to keep the temp-folders (`-k`)
* added colors

### v1.0.2d (01.06.2019)

* fixed spelling in translations
* added create argument for creation of package YAMLs
* added new translation sentences

### v1.0.2c (11.05.2019)

* fixed bug at too-few-arguments-error
* fixed wrong spelling in translations
* added search-script for BitBucket

### v1.0.2b (09.05.2019)

* fixed bug in search-online argument

### v1.0.2a (09.05.2019)

* fixed installation bug
* fixed bug in help page generation
* added search-online function for GitHub and GitLab
* renamed old search argument (for installed packages) from "s" to "si"

### v1.0.1a (24.04.2019)

* fixed installation bug for choosing the language
* added branching support
* added logo to source
* added new translation sentences

### v1.0.0a (18.04.2019)

* added multi language support
* added Tridymite installation script
* added basic functionality (install, remove, update)
* added support for local installations
* added basic PreType support