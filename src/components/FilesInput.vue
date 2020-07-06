<template>
  <div style="width: 600px; display: flex; flex-direction: column; justify-content: flex-end">

    <v-card elevation="4" class="card">
      <input-file
        v-for="(file, i) of files"
        :file="file"
        :key="i"
        @view="readFile(i)"
        @remove="files.splice(i)"
      />

      <v-btn fab small color="orange" dark class="attach-btn" @click="$refs.file.click()">
        <input ref="file" type="file" accept="text/csv" style="display: none" @change="filePicked" />
        <v-icon>mdi-paperclip</v-icon>
      </v-btn>
    </v-card>

    <v-card class="card" style="padding-bottom: 20px !important;">
      <v-text-field
        dense
        outlined
        v-model="size"
        label="Phrase length"
        :rules="[v => (v > 0 && v <= 7) || 'You have entered an invalid value']"
        hint="Max length of 7 words is supported currently"
      />
      <v-textarea dense outlined disabled hide-details placeholder="Not Available in this release" />
    </v-card>

    <v-btn width="100%" elevation="0" color="primary" :disabled="files.length === 0" @click="readAllFiles">Analyze</v-btn>

    <v-dialog v-model="state.view" :width="state.viewLoading ? 300: 900">
      <v-card
        v-if="state.viewLoading"
        height="150"
        style="display: flex; justify-content: center; align-items: center; flex-direction: column">
        <v-progress-circular indeterminate color="primary" size="50" />
        <span style="margin-top: 20px">Reading File... </span>
      </v-card>

      <v-data-table
        v-else
        height="500"
        fixed-header
        :items="state.viewData"
        :headers="state.viewHeader"
      />
    </v-dialog>

    <v-dialog v-model="state.error">
      {{ state.errorMessage }}
    </v-dialog>
  </div>
</template>

<script>
import csv from 'csv-parser'
import InputFile from "./InputFile"
import { exec } from 'child_process'
import { createReadStream, createWriteStream } from 'fs'

export default {
  name: "FilesInput",

  components: {
    InputFile
  },

  data: () => ({
    size: 0,
    files: [],

    state: {
      view: false,
      viewData: [],
      viewHeader: [],
      viewLoading: true,

      error: false,
      errorMessage: ''
    }
  }),

  methods: {
    filePicked(event) {
      if (event.target.files.length > 0) {
        let flag = false

        for (let i = 0; i < event.target.files.length; ++i) {
          if (this.files.length > 0) {
            for (const file of this.files) {
              if (file.path === event.target.files[i].path)
                flag = true
            }
          }

          if (!flag) {
            this.files.push({
              name: event.target.files[i].name,
              path: event.target.files[i].path,
              size: event.target.files[i].size
            })
          }
        }

        event.target.value = ''
      }
    },

    readFile(index) {
      const data = []
      this.state.view = true
      this.state.viewLoading = true

      createReadStream(this.files[index].path)
        .pipe(csv())
        .on('data', d => data.push(d))
        .on('end', () => {
          if (data.length > 0) {
            this.state.viewHeader = Object.keys(data[0]).map(item => ({
              value: item,
              text: item
            }))
          }

          this.state.viewData = data
          this.state.viewLoading = false
        })
    },

    readAllFiles() {
      this.state.view = true
      this.state.viewLoading = true
      const write = createWriteStream('output')

      for (let i = 0; i < this.files.length; ++i) {
        createReadStream(this.files[i].path)
          .pipe(csv())
          .on('data', d => {
            write.write(" " + d["text"].toLowerCase() + " " + d["title"].toLowerCase() + " ")
          })
          .on('end', () => {
            if (i === this.files.length - 1) {
              write.end()

              exec(`/home/arish/Workspace/Tools/C++/review_analysis/a.out output ${this.size} ${Date.now()}`, () => {
                this.state.view = false
              })
            }
          })
      }
    }
  }
}
</script>

<style lang="sass" scoped>
.card
  width: 100%
  padding: 20px 20px 30px
  position: relative
  margin-bottom: 30px

.attach-btn
  right: 15px
  bottom: -20px
  position: absolute
</style>