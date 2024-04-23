from setuptools import setup, find_packages

with open("Readme.md", "r") as fh:
    long_description = fh.read()

opts = dict(
    name="romi",
    packages=find_packages(),
    version='0.1',
    author='Peter Hanappe',
    author_email='hanappe@csl.sony.fr',
    description='Python package for the ROMI hardware devices.',
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/romi/libromi/",
    use_scm_version=False,
    setup_requires=['setuptools_scm'],
    install_requires=[
        'pyserial',
        'asyncio',
        'websockets'
    ],
    python_requires='>=3.6',
    classifiers=[
        "Programming Language :: Python :: 3",
        "Development Status :: 4 - Beta",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: GNU Lesser General Public License v3 or later (LGPLv3+)"
    ],
    # If True, include any files specified by your MANIFEST.in:
    include_package_data=False
)

if __name__ == '__main__':
    setup(**opts)
