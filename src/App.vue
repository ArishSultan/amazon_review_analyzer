<template>
  <v-app>
    <v-app-bar app dense elevation="0" color="primary"></v-app-bar>
    <v-main>
      <v-container>
        <files-input />
      </v-container>
    </v-main>
  </v-app>
</template>

<script>
import FilesInput from "./components/FilesInput";

export default {
  components: {
    FilesInput
  }
}
</script>

<style>
html {
  overflow-y: hidden !important;
}

.container {
  display: flex !important;
  justify-content: center !important;
}
</style>