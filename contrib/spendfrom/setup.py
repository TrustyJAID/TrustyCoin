from distutils.core import setup
setup(name='btcspendfrom',
      version='1.0',
      description='Command-line utility for trustycoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@trustycoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
