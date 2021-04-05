format_in := md
format_out := html

dir_stylesheet := styles
stylesheet_articles := notes.css
stylesheet_reader := reader.css
stylesheet_misc := core.css
stylesheets := $(addprefix $(dir_stylesheet)/, $(stylesheet_articles) $(stylesheet_misc) )

args_pandoc := -s -c $(dir_stylesheet)/$(stylesheet_articles) --self-contained --toc --toc-depth=3 --highlight-style monochrome

.PHONY: all

all: $(shell ls $(format_in)/*.$(format_in) | sed "s/$(format_in)/$(format_out)/g")

# Sem?/[Fach]/html
$(format_out): 
	mkdir $@

$(format_out)/%.$(format_out): $(format_in)/%.$(format_in) $(stylesheets) | $(format_out)
	pandoc $< $(args_pandoc) -o $@ 
