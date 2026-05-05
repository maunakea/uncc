# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

A static HTML personal/academic portfolio site for Satish Narayan's UNCC Master of Computer Science work (graduated May 2001). No build tools, no package managers, no compilation — everything is plain HTML, GIF images, and one JavaScript file.

## Deployment

Pushing to `main` triggers `.github/workflows/static.yml`, which deploys the entire repository to GitHub Pages. There is no local build step — open any `.html` file directly in a browser to preview.

## Architecture

The site uses a CSS flexbox layout with a fixed left sidebar and an `<iframe>` content area:

- `index.html` — entry point; 120px left sidebar (flex column) + full-height `<iframe name="content">` on the right. The sidebar has three zones matching the old frameset rows: a 70px top (home button), flexible middle (`#toc` div), and 160px bottom (email/UNCC links).
- `displayToc.js` — all navigation logic. `reDisplay(selection)` builds an HTML string and sets `document.getElementById('toc').innerHTML`. `loadContent(url)` sets `document.getElementById('content').src`. Adding or changing nav sections requires editing this file.
- `home.html` — default page loaded in the content iframe on first load.
- `courses_summary.html`, `projects_summary.html`, `resume/resume_summary.html` — summary pages loaded into the iframe when top-level nav items are clicked.

Each course has its own subdirectory (`csci5102/`, `csci5112/`, `csci5130/`, `csci5141/`, `csci6114/`, `csci6144/`, `csci6148/`, `csci6160/`, `csci6162/`, `csci6166/`) containing an HTML overview page plus any project files.

The files `tocHead.html`, `tocTail.html`, and `blank.html` are legacy remnants from the old frameset — their content is now inlined in `index.html` and they are no longer used.

## Making changes

- **New course page**: create a subdirectory with an HTML file, then add an `<a>` entry inside the `"courses"` branch of `reDisplay()` in `displayToc.js`, following the same `onclick="loadContent(...)"` pattern.
- **New top-level nav section**: add image preload variables at the top of `displayToc.js`, add the nav button block in `reDisplay()`, and create a corresponding summary HTML page.
- **Resume formats**: files live in `resume/`; the ToC links in `displayToc.js` reference `resume.html`, `resume.doc`, and `resume.txt` by name.
