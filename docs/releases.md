# Releases

## Versions

Any release on GitHub, if there is any, might be behind the latest code. Prefer
to clone or fork the master branch instead of downloading the releases. If you
find any back compatibility issue which is not announced, please report a bug.

!!! note "Bugs and issues"

    [Issues][cvop-issues] in the CVIP-operator repository are the common way to
    raise an information.

## Release cycle

[The CVIP++ Project][cvip-project] has no strict schedule for releasing. Other
features will come in the next versions and you can propose new features through
[project issues][cvip-issues]. Each feature is discussed in a separate issue.

### Version 0.1.0

The master branch is usable and stable.

## Change log

For a full overview of changes, you can peek at the commit history.

**Version 0.1.0** - 16 July, 2022  
First version  
Adds `:::cpp i_operator`, `:::cpp i_operator_predicate`,
`:::cpp base_operator<T>`, `:::cpp basic_operator<T>`,
`:::cpp operator_expression` and related `:::cpp operator*(_, _)`,
all in the `cvip::core` namespace.

### To dos

- [ ] Remove items not achieved from Build instructions before publish this
  README.
- [ ] Add example for the operator() overload for setting operation parameters.
- [ ] Add CMake scripts.
- [ ] Add API documentation.
- [ ] Copy installation directives in its own INSTALL.md file in project root.

[cvip-project]: https://github.com/wvenialbo/CVIP "The CVIP++ Project"
[cvip-issues]: https://github.com/wvenialbo/CVIP/issues "The CVIP++ Project issues"
[cvop-issues]: https://github.com/wvenialbo/CVIP-Operator/issues "CVIP-Operator issues"
