#!/usr/bin/env python3.5

import yaml
import os
import subprocess


def git(*args):
    return subprocess.call(['git'] + list(args))


def run_cmd(cmd):
    os.system(cmd)


def run(cfg):

    path = 'dependencies'

    if not os.path.isdir(path):
        os.makedirs(path)

    root = os.getcwd()
    os.chdir(path)

    for name, module in cfg['dependencies'].items():

        print('\x1b[6;30;42m ' + name + ' \x1b[0m')

        if not os.path.isdir(name):
            if 'git' in module:
                git('clone', module['git']['url'], name)
                if 'command' in module['git']:
                    run_cmd(module['git']['command'])

            if 'download' in module:
                run_cmd('curl -OL ' + module['download']['url'])
                if 'command' in module['download']:
                    run_cmd(module['download']['command'])

        if os.path.isdir(name):

            prev = os.getcwd()
            os.chdir(name)

            if 'git' in module and 'checkout' in module['git']:
                git('checkout', module['git']['checkout'])

            if 'make' in module:
                make = module['make']
                if 'dir' in make:
                    build_dir = make['dir']
                    if not os.path.isdir(build_dir):
                        os.makedirs(build_dir)
                    os.chdir(build_dir)
                if 'command' in make:
                    run_cmd(make['command'])
                if 'commands' in make:
                    for command in make['commands']:
                        run_cmd(command)

            os.chdir(prev)

        print('\x1b[6;30;42m done. \x1b[0m')

    os.chdir(root)

    if 'target' in cfg:
        if 'commands' in cfg['target']:
            for command in cfg['target']['commands']:
                run_cmd(command)


with open('packmake.yaml', 'r') as stream:
    try:
        run(yaml.load(stream))
    except yaml.YAMLError as exc:
        print(exc)
