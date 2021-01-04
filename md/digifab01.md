% Introduction to documentation

## Documentation using Markdown

> This manual was set by a Graphic Systems phototypesetter driven by the troff formatting program operating under the UNIX system. The text of the manual was prepared using the ed text editor.

- Thompson, K. and Ritche, D.M. (1973) Unix Programmer's Manual. 4th edn. Bell Telephone Laboratories, Inc.

There are countless ways of writing good documentation. From good ol' pen and paper to sophisticated digital formats like PDF LaTeX or HTML. We're gonna use Markdown, as it is a lightweight, plaintext-oriented format which can easily be suited towards various needs using simple automation.

For presentation within fundamentals of digital fabrication this documentation's Markdown source will be converted into HTML using GNU Make, the Pandoc parser and a CSS stylesheet for typesetting.

### Markdown

If you wanna read Markdown, don't worry about it as Markdown was developed to be especially readable in plaintext/source form. If you want an example, all sources are available on [this very website's GitHub repository](https://github.com/zibonbadi/zibonbadi.github.io).

Should you need a refresher on *writing* Markdown, I recommend reading [this cheat sheet](https://www.markdownguide.org/cheat-sheet).

### Pandoc

[Pandoc](https://pandoc.org/) is a command-line parser built for converting various document types and markup languages into each other. It is available for all common operating systems, as well as Windows.

The following command was used to convert these Markdown notes into HTML pages:

	pandoc md/file.md -s -c ../styles/style.css --toc --toc-depth=3 --highlight-style monochrome -o html/file.html

The `-s` option stands for *standalone*, meaning that certain base elements like the tag `<head>` are included in the final page.
The `-c` option links a CSS sheet for various typesetting enhancements to increase readability.
The flags `--toc` and `--toc-depth 3` include an auto-generated table of contents at the top of the page, as there is currently no standard way to let the web browser generate one.
`--highlight-style` monochrome includes an auto-generated syntax highlighting style in the document for any code snippets found. *Monochrome* was chosen as it is simple and readable without clashing with the included custom stylesheet.

### Make

[GNU Make](https://www.gnu.org/software/make/) is a libre implementation of Make, a system originally developed for the BSD operating system to automate the creation of programs, especially those written in C.

Make's concept is fairly simple: Make checks a so-called *Makefile* for predefined rules regarding certain files. Should a createable file not exist or be updated compared to files that depend on it, Make will execute predefined commands to recreate all necessary files.

Given everything mentioned above, below you can see the contents of the Makefile used to create individual project documentations:

```Make
format_in := md
format_out := html

dir_stylesheet := styles
stylesheet_articles := notes.css
stylesheet_reader := reader.css

args_pandoc := -s -c ../$(dir_stylesheet)/$(stylesheet_articles) --toc --toc-depth=3 --highlight-style monochrome

.PHONY: all

all: $(shell ls $(format_in)/*.$(format_in) | sed "s/$(format_in)/$(format_out)/g")

# Sem?/[Fach]/html
$(format_out):
	mkdir $@

$(format_out)/%.$(format_out): $(format_in)/%.$(format_in) | $(format_out)
	pandoc $< $(args_pandoc) -o $@ 
```

Simply put: Make checks the directory *md* (short for Markdown) for updated documentation and executes Pandoc to generate HTML files into the directory *html*. For more on the exact Pandoc setup, see the section *Pandoc*.

## Project documentation using Git

> I'm an egotistical bastard, and I name all my projects after myself. First Linux, now git.

- Linus Torvalds

All project documentations on this webpage have been progressively extended throughout the module using the Git version control system. Since people attending this class will most likely work using Windows, it is generally advised to clone the documentation repository using an SSH handle, since as of the writing of this document (November 2020) Git for Windows is reported to have security issues around HTTP connections.

	git clone git@repository:user/repository

In order to use Git over SSH you will need to generate an public-private key pair for every machine you intend on using and register the public key in your Git repository.

	ssh-keygen -t rsa -C email@address

Depending on your operating system, this program will create an RSA key pair based on the supplied email address. The keys will be supplied in plain text, such that you need to open the public key file and supply it's contents to your Git repository's SSH verification. Details on how to do this vary by vendor.

### Git

[Git](https://git-scm.com/) is a version control system originally conceptualized by [Linus Torvalds]( https://en.wikipedia.org/wiki/Linus_Torvalds ) in order to manage the large-scale, decentralized development of the Linux kernel. It has since become widely used among software developers, especially among open-source projects.

In human words: We're gonna use Git to keep track of changes made to this website, especially as more documentation gets added later on.

For people unfamiliar with Git, after cloning you might want to update your repository using `git pull`.

After you're done with your changes, you need to add them to the change history using `git add .` and file a commit. Usually I use the following command:

```Bash
git commit -m "Whatever changes I made"
```

Lastly, you need to "push" your updates to the remote server, as git is based on "clones", meaning local copies of the source tree. One important thing to note here is that GitHub - unlike many other git repositories - prefers to call their root branches *main* rather than *master*.

```Bash
git push origin main
```

### GitHub

GitHub allows for easy hosting of custom websites for documentation purposes. All that is needed is to create a repository called `username.github.io` with an *index.html* file and it will be publicly viewable on *username.github.io*

Interesting to note here is that you could also write your website into the standard *README.md*, as GitHub will use it's internal Markdown parser to display it's contents.
