'use strict'

const webpack = require('webpack')
const path = require('path')

module.exports = {
  mode: 'production',
  entry: [path.resolve('../.bootstrap/index.js'), path.resolve('index.js')],
  output: {
    path: path.resolve('dist'),
    filename: 'index.js'
  },
  node: false,
  resolve: {
    modules: ['node_modules']
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /(node_modules|bower_components)/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: [path.join(__dirname, 'node_modules/babel-preset-env')]
          }
        }
      }
    ]
  }
}
