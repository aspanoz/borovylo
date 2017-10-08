'use strict';

const gulp = require('gulp');
const babel = require('gulp-babel');
const chokidar = require('chokidar');
const replace = require('gulp-replace');

const {argv} = require('yargs');
const {exec, spawn} = require('child_process');

const DEBUG = false;

/*
 *   C++ task. Clean. Cmake. Make
 */
const outHandler = (error, stdout, stderr) => {
  if (stderr) { console.log(stderr); }
  if (DEBUG) { console.log(stdout); }
}

const tasks = {
  cmake: 'mkdir build && cd build && cmake ..',
  clean: `if [ -d build ]; then rm -fR build; fi`,
  build: 'cd build; make'
};

for(let t in tasks) gulp.task(t, () => exec(`${tasks[t]} || exit 0`, outHandler));

/*
 *  Generate files for neovim plugins configs
 */

gulp.task('copy_compile_commands', () => {
  const {path} = watches['neovim_config'];
  return gulp.src(`${__dirname}/${path}`).pipe(gulp.dest(__dirname));
});

gulp.task('make_local_config', () => {
  // Get include path from cmake generated compile_commands.json
  const [ compile_commands ] = require(`${__dirname}/compile_commands.json`);
  const cmake_include_path = compile_commands.command.split(' ').filter((s) => /^-I/.test(s)).join(' ');

  console.log('Include path: ', cmake_include_path);

  if (DEBUG) {
    console.log('Include path: ', cmake_include_path);
  }


  return gulp.src(`${__dirname}/cmake/.lvimrc`)
    .pipe(replace('@CMAKE_INCLUDE_PATH@', cmake_include_path))
    .pipe(gulp.dest(`${__dirname}`));
});

/*
 *  Watch task
 */
const watches = {
  neovim_config: {
    path: 'build/compile_commands.json',
    task: ['copy_compile_commands', 'make_local_config'],
    event: 'add'
  },
  code: { 
    path: 'src/**/*.*',
    task: ['build'],
    event: 'change'
  },
  cmake_config: {
    path: ['CMakeLists.txt', 'cmake/**/*.*'],
    task: ['clean', 'cmake', 'build'],
    event: 'change'
  }
}


gulp.task('watch', () => {
  for(let w in watches) {
    const {path, task, event} = watches[w];
    let chokowatcher = chokidar.watch(path).on(event, gulp.series(...task));

    if (DEBUG) {
      chokowatcher.on('all', (event, path) => {console.log(`-- Gulp watch event: ${event}, path: ${path}`);});
    }
  }
});


/*
 *  Gulp default 
 */
gulp.task('default', gulp.series('clean', 'cmake', 'build', 'watch'));


/*
 *  Reload gulp process on gulpfile.js change
 */
gulp.task('auto-reload', () => {
  let p;
  let spawnChildren = () => {
    if (p) { p.kill(); }
    p = spawn('gulp', [argv.task], {stdio: 'inherit'});
  }

  chokidar.watch('./gulpfile.js').on('all', spawnChildren);
  spawnChildren();
});
