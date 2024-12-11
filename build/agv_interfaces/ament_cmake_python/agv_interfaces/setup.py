from setuptools import find_packages
from setuptools import setup

setup(
    name='agv_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('agv_interfaces', 'agv_interfaces.*')),
)
