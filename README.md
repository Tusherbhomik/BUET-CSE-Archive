# BUET CSE Archive

A single, structured home for coursework, sessional projects, and reference material from the
BUET CSE curriculum, consolidated from 14 previously scattered GitHub repositories.

This repo was assembled by copying content from the original repos (no file content was
modified), organizing it by course level, and removing duplicate/regenerable files along the way.
Each original repo is still available on GitHub with its full commit history — see the mapping
table below.

## Structure

```
level-1/
  CSE-108-OOP-JavaFlix/                    CSE 108 - OOP Sessional project
level-2/
  CSE-203-207-DSA/                         CSE 203/204/207/208 - DSA I & II, slides + solutions
level-3/
  CSE-307-308-Software-Engineering/        CSE 307/308 - Software Engineering Sessional
  CSE-309-310-Compiler/                    CSE 309/310 - Compiler Sessional
  CSE-313-314-Operating-System/            CSE 313/314 - Operating System Sessional
  CSE-317-318-Artificial-Intelligence/     CSE 317/318 - AI Sessional
  CSE-321-322-Computer-Networks/           CSE 321/322 - Computer Networks Sessional
level-4/
  CSE-405-406-Computer-Security/           CSE 405/406 - Computer Security Sessional
  CSE-409-410-Computer-Graphics/           CSE 409/410 - Computer Graphics Sessional
  CSE-461-462-Algorithm-Engineering/
    project-mkp/                           CSE 461/462 sessional project (Multi-dim Knapsack)
    reference-books/                       Reference textbooks for the course
  CSE-471-472-Machine-Learning/            CSE 471/472 - Machine Learning Sessional project
resources/
  ConcreteMathematics/                     Supplementary math course material
```

## Repo mapping (source of full history)

| Folder here | Original repo |
|---|---|
| `level-1/CSE-108-OOP-JavaFlix` | [CSE_108_OOP_Project_JavaFlix](https://github.com/Tusherbhomik/CSE_108_OOP_Project_JavaFlix) |
| `level-2/CSE-203-207-DSA` | [DSA_SLIDES](https://github.com/Tusherbhomik/DSA_SLIDES) |
| `level-3/CSE-307-308-Software-Engineering` | [CSE_308-SWE-Sessional](https://github.com/Tusherbhomik/CSE_308-SWE-Sessional) |
| `level-3/CSE-309-310-Compiler` | [CSE-310-Compiler-Design](https://github.com/Tusherbhomik/CSE-310-Compiler-Design) |
| `level-3/CSE-313-314-Operating-System` | [CSE-314-Operating-System](https://github.com/Tusherbhomik/CSE-314-Operating-System) |
| `level-3/CSE-317-318-Artificial-Intelligence` | [CSE-318-Artificial-Engineering-](https://github.com/Tusherbhomik/CSE-318-Artificial-Engineering-) |
| `level-3/CSE-321-322-Computer-Networks` | [CSE_322-Networking](https://github.com/Tusherbhomik/CSE_322-Networking) |
| `level-4/CSE-405-406-Computer-Security` | [Computer-Security-Sessional-406](https://github.com/Tusherbhomik/Computer-Security-Sessional-406) |
| `level-4/CSE-409-410-Computer-Graphics` | [Computer-Graphics-Sessional-410](https://github.com/Tusherbhomik/Computer-Graphics-Sessional-410) |
| `level-4/CSE-461-462-.../project-mkp` | Merged from [Algorithm-Sessional-Project--461](https://github.com/Tusherbhomik/Algorithm-Sessional-Project--461) (base) + 2 unique files from [Algorithm-Sessional-Project-461](https://github.com/Tusherbhomik/Algorithm-Sessional-Project-461) |
| `level-4/CSE-461-462-.../reference-books` | [CSE462-Algorithm-Engineering-Sessional](https://github.com/Tusherbhomik/CSE462-Algorithm-Engineering-Sessional) |
| `level-4/CSE-471-472-Machine-Learning` | [ML-Project-472](https://github.com/Tusherbhomik/ML-Project-472) |
| `resources/ConcreteMathematics` | [ConcreteMathematics](https://github.com/Tusherbhomik/ConcreteMathematics) |

Not merged in: **[BUET_CSE_ACADEMICS](https://github.com/Tusherbhomik/BUET_CSE_ACADEMICS)** (1.24 GB of
course slides/books uploaded via the GitHub web UI) — kept separate due to its size; linked here for
discoverability.

## Deduplication notes

- **`Algorithm-Sessional-Project-461` vs `--461`**: these were two copies of the same sessional
  project (421 of 423 files identical by content). Merged into `project-mkp/` using `--461` as the
  base (more commits) plus the 2 files only present in the single-dash copy
  (`Presentation/Omega_Group_9.pdf`, `report/Project_Report.pdf`).
- **Within-repo duplicate files** (same file saved in two places in the same original repo, e.g.
  loose working files also copied into a submission subfolder) were deduplicated, keeping the
  shallowest path. Affected `Computer-Graphics-Sessional-410` (Offline 3 Ray Tracing) and
  `DSA_SLIDES` (BalancedBST).
- **Build artifacts / IDE output** (`.class`, `.exe`, `.o`, `bin/`, `build/`, `.gradle/`, `.idea/`)
  were dropped as regenerable, not source content.
- Roughly 130 MB of redundant/regenerable data was removed in total; nothing unique was lost — all
  original repos remain intact on GitHub as the historical record.

## Course reference

See [`BUET_CSE_COURSES.txt`](./BUET_CSE_COURSES.txt) for the full BUET CSE curriculum course list
(course codes, titles, credit hours).

## Visibility

This repo is **private** — several folders contain copyrighted textbooks/solution manuals
(*Concrete Mathematics*, Sheldon Ross, *Encyclopedia of Algorithms*, CORMEN solutions) and private
coursework, matching the visibility of their original source repos.
