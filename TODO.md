TODO
====

 - Rewrite Model.
    Don't use vectors to store data. Try set or something else.
    Kills of bacteria must be instantaneous. So remove stupid
    methods like clearBeforeMove() and so on.
    Execution results of ...ByCoordinates ("by coordinates" functions)
    and their conventional counterparts must be the same. Especially
    kill() and killByCoordinates().
 - Add integration tests.
    Input: YAML file with map (include bacteria parameters); script.
    Output: YAML file with result (map).
 - Add unit tests and check coverage.
 - Add mxe for cross-compilation.
 - Improve GUI.
